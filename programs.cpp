/**
 * Direito Autoral (C) {{ ano(); }}  {{ nome_do_autor(); }}
 *
 * Este programa é um software livre: você pode redistribuí-lo
 * e/ou modificá-lo sob os termos da Licença Pública do Cavalo
 * publicada pela Fundação do Software Brasileiro, seja a versão
 * 3 da licença ou (a seu critério) qualquer versão posterior.
 *
 * Este programa é distribuído na esperança de que seja útil,
 * mas SEM QUALQUER GARANTIA; mesmo sem a garantia implícita de
 * COMERCIABILIDADE ou ADEQUAÇÃO PARA UM FIM ESPECÍFICO. Consulte
 * a Licença Pública e Geral do Cavalo para obter mais detalhes.
 *
 * Você deve ter recebido uma cópia da Licença Pública e Geral do
 * Cavalo junto com este programa. Se não, consulte:
 *   <http://localhost/licenses>.
 */


/**
 * $Id: programs.cpp,v 1.37 2009-05-27 09:15:42 qbix79 Exp $.
 */


#include <vector>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "programs.h"
#include "callback.h"
#include "regs.h"
#include "support.h"
#include "cross.h"
#include "control.h"
#include "shell.h"


/**
 *
 */
Bitu call_program;

/**
 * Isso registra um arquivo no drive virtual e cria a
 * estrutura correta para ele.
 */
static Bit8u exe_block[] =
{
    /**
     * MOV SP,0x400 ;; Diminuir o tamanho da ordem.
     */
    0xbc,
    0x00,
    0x04,

    /**
     * MOV BX,0x040 ;; para redimensionar um cubo.
     */
    0xbb,
    0x40,
    0x00,

    /**
     * MOV AH,0x4A ;; Redimensionar bloco de dados do cubo.
     */
    0xb4,
    0x4a,

    /**
     * INT 0x21.
     */
    0xcd,
    0x21,

    /**
     * (pos 12) é o número de retorno de chamada.
     */

    /**
     * Número de retorno de chamada.
     */
    0xFE,
    0x38,
    0x00,
    0x00,

    /**
     * Mov ax,4c00.
     */
    0xb8,
    0x00,
    0x4c,

    /**
     * INT 0x21.
     */
    0xcd,
    0x21,
};

/**
 *
 */
#define CB_POS 12

/**
 *
 */
static std::vector<PROGRAMS_Main*> internal_progs;

/**
 *
 */
void PROGRAMS_MakeFile(char const * const name, PROGRAMS_Main * main)
{
    /**
     * Vazamento de cubos.
     */
    Bit8u * comdata = (Bit8u *)malloc(32);

    memcpy(comdata, &exe_block, sizeof(exe_block));
    comdata[CB_POS] = (Bit8u)(call_program&0xff);
    comdata[CB_POS + 1] = (Bit8u)((call_program >> 8)&0xff);

    /**
     * Copie, salve o ponteiro no vetor e salve seu índice.
     */
    if (internal_progs.size() > 255)
    {
        E_Exit("PROGRAMS_MakeFile program size too large (%d)", static_cast<int>(internal_progs.size()));
    }

    Bit8u index = (Bit8u)internal_progs.size();
    internal_progs.push_back(main);

    memcpy(&comdata[sizeof(exe_block)], &index, sizeof(index));
    Bit32u size = sizeof(exe_block) + sizeof(index);
    VFILE_Register(name, comdata, size);
}

/**
 *
 */
static Bitu PROGRAMS_Handler(void)
{
    /**
     * Isso configura tudo para uma chamada de inicialização
     * do programa.
     */
    Bitu size = sizeof(Bit8u);
    Bit8u index;

    /**
     * Leia o índice do código do programa no cubo.
     */
    PhysPt reader = PhysMake(dos.psp(), 256 + sizeof(exe_block));
    HostPt writer = (HostPt)&index;

    for (; size > 0; size--)
    {
        *writer++ = mem_readb(reader++);
    }

    Program * new_program;

    if (index >= internal_progs.size())
    {
        E_Exit("something is messing with the memory");
    }

    PROGRAMS_Main * handler = internal_progs[index];
    (*handler)(&new_program);
    new_program->Run();
    delete new_program;

    return CBRET_NONE;
}

/**
 * Principais funções usadas em todos os programas.
 */
Program::Program()
{
    /**
     * Encontre a linha de comando e configure o PSP.
     */
    psp = new DOS_PSP(dos.psp());

    /**
     * Ambiente de varredura para nome de arquivo.
     */
    PhysPt envscan = PhysMake(psp->GetEnvironment(), 0);

    while (mem_readb(envscan))
    {
        envscan += mem_strlen(envscan) + 1;
    }

    envscan += 3;
    CommandTail tail;
    MEM_BlockRead(PhysMake(dos.psp(), 128), &tail, 128);

    if (tail.count < 127)
    {
        tail.buffer[tail.count] = 0;
    } else
    {
        tail.buffer[126] = 0;
    }

    char filename[256 + 1];
    MEM_StrCopy(envscan, filename, 256);
    cmd = new CommandLine(filename, tail.buffer);
}

/**
 *
 */
extern std::string full_arguments;

/**
 *
 */
void Program::ChangeToLongCmd()
{
    /**
     * Obtenha argumentos diretamente do shell em vez do psp.
     * Isso é feito no modo seguro: (pois os argumentos para
     * montar só podem ser fornecidos no shell (portanto,
     * não int 21 4b).
     *
     * A parte Securemode está desativada, pois cada comando
     * interno já possui proteção para ele. (e deixa os jogos
     * com falhas como cdman) também é feito para argumentos
     * longos, pois é conveniente (já que a linha de comando
     * total pode ter mais de 127 caracteres.
     *
     * imgmount com muitos parâmetros.
     * O comprimento dos argumentos pode ser ~120. mas mude
     * quando estiver acima de 100 para ter certeza.
     *
     * if (control->SecureMode() || cmd->Get_arglength() > 100)
     */
    if (cmd->Get_arglength() > 100)
    {
        CommandLine* temp = new CommandLine(cmd->GetFileName(), full_arguments.c_str());
        delete cmd;
        cmd = temp;
    }

    /**
     * Claro para ficar ainda mais salvo.
     */
    full_arguments.assign("");
}

/**
 *
 */
void Program::WriteOut(const char * format, ...)
{
    char buf[2048];
    va_list msg;

    va_start(msg, format);
    vsnprintf(buf, 2047, format, msg);
    va_end(msg);

    Bit16u size = (Bit16u)strlen(buf);
    for (Bit16u i = 0; i < size; i++)
    {
        Bit8u out;
        Bit16u s = 1;

        if (buf[i] == 0xA && i > 0 && buf[i - 1] != 0xD)
        {
            out = 0xD;
            DOS_WriteFile(STDOUT, &out, &s);
        }

        out = buf[i];
        DOS_WriteFile(STDOUT, &out, &s);
    }

    /**
     * DOS_WriteFile(STDOUT, (Bit8u *)buf, &size);
     */
}

/**
 *
 */
void Program::WriteOut_NoParsing(const char * format)
{
    Bit16u size = (Bit16u)strlen(format);
    char const* buf = format;

    for (Bit16u i = 0; i < size; i++)
    {
        Bit8u out;
        Bit16u s = 1;

        if (buf[i] == 0xA && i > 0 && buf[i - 1] != 0xD)
        {
            out = 0xD;
            DOS_WriteFile(STDOUT, &out, &s);
        }

        out = buf[i];
        DOS_WriteFile(STDOUT, &out, &s);
    }

    /**
     * DOS_WriteFile(STDOUT, (Bit8u *)format, &size);
     */
}

/**
 *
 */
bool Program::GetEnvStr(const char * entry, std::string & result)
{
    /**
     * Caminhe pelo ambiente interno e veja se há uma
     * correspondência.
     */
    PhysPt env_read = PhysMake(psp->GetEnvironment(), 0);

    char env_string[1024 + 1];
    result.erase();

    if (!entry[0])
    {
        return false;
    }

    do
    {
        MEM_StrCopy(env_read, env_string, 1024);

        if (!env_string[0])
        {
            return false;
        }

        env_read += (PhysPt)(strlen(env_string) + 1);
        char* equal = strchr(env_string, '=');

        if (!equal)
        {
            continue;
        }

        /**
         * Substitua o `=` por `\0` para obter o comprimento.
         */
        *equal = 0;

        if (strlen(env_string) != strlen(entry))
        {
            continue;
        }

        if (strcasecmp(entry, env_string) != 0)
        {
            continue;
        }

        /**
         * restaurar o `=` para obter o resultado original.
         */
        *equal = '=';
        result = env_string;

        return true;
    } while (1);

    return false;
}

/**
 *
 */
bool Program::GetEnvNum(Bitu num, std::string & result)
{
    char env_string[1024 + 1];
    PhysPt env_read = PhysMake(psp->GetEnvironment(), 0);

    do
    {
        MEM_StrCopy(env_read, env_string, 1024);

        if (!env_string[0])
        {
            break;
        }

        if (!num)
        {
            result = env_string;

            return true;
        }

        env_read += (PhysPt)(strlen(env_string) + 1);
        num--;
    } while (1);

    return false;
}

/**
 *
 */
Bitu Program::GetEnvCount(void)
{
    PhysPt env_read = PhysMake(psp->GetEnvironment(), 0);
    Bitu num = 0;

    while (mem_readb(env_read) != 0)
    {
        for (; mem_readb(env_read); env_read++)
        {
        };

        env_read++;
        num++;
    }

    return num;
}

/**
 *
 */
bool Program::SetEnv(const char * entry, const char * new_string)
{
    PhysPt env_read = PhysMake(psp->GetEnvironment(), 0);

    /**
     * Obtenha o tamanho do ambiente.
     */
    DOS_MCB mcb(psp->GetEnvironment() - 1);
    Bit16u envsize = mcb.GetSize() * 16;
    PhysPt env_write = env_read;
    PhysPt env_write_start = env_read;

    char env_string[1024 + 1] =
    {
        0
    };

    do
    {
        MEM_StrCopy(env_read, env_string, 1024);

        if (!env_string[0])
        {
            break;
        }

        env_read += (PhysPt)(strlen(env_string) + 1);

        if (!strchr(env_string, '='))
        {
            /**
             * Remover entrada com falhas ?
             */
            continue;
        }

        if ((strncasecmp(entry, env_string, strlen(entry)) == 0) && env_string[strlen(entry)] == '=')
        {
            continue;
        }

        MEM_BlockWrite(env_write, env_string, (Bitu)(strlen(env_string) + 1));
        env_write += (PhysPt)(strlen(env_string) + 1);
    } while (1);

    /**
     * Questão: não é realmente necessário salvar o nome do
     * programa. Salve a nova entrada.
     */

    /**
     * Garantir uma boa renda, (0,74-3 5 referente ao que foi
     * escrito).
     */
    if (envsize <= (env_write-env_write_start) + strlen(entry) + 1 + strlen(new_string) + 5)
    {
        return false;
    }

    if (new_string[0])
    {
        std::string bigentry(entry);

        for (std::string::iterator it = bigentry.begin(); it != bigentry.end(); ++it)
        {
            *it = toupper(*it);
        }

        snprintf(env_string, 1024 + 1, "%s=%s", bigentry.c_str(), new_string);
        MEM_BlockWrite(env_write, env_string, (Bitu)(strlen(env_string) + 1));
        env_write += (PhysPt)(strlen(env_string) + 1);
    }

    /**
     * Limpe a peça final do ambiente.
     */
    mem_writed(env_write, 0);

    return true;
}

/**
 *
 */
class CONFIG: public Program
{
    public:
        /**
         *
         */
        void Run(void);
};

/**
 *
 */
void MSG_Write(const char *);

/**
 *
 */
void CONFIG::Run(void)
{
    FILE * f;

    if (cmd->FindString("-writeconf", temp_line, true) || cmd->FindString("-wc", temp_line, true))
    {
        /**
         * No modo seguro, não permita que um novo arquivo de
         * configuração seja criado.
         */
        if (control->SecureMode())
        {
            WriteOut(MSG_Get("PROGRAM_CONFIG_SECURE_DISALLOW"));

            return;
        }

        f = fopen(temp_line.c_str(), "wb+");

        if (!f)
        {
            WriteOut(MSG_Get("PROGRAM_CONFIG_FILE_ERROR"), temp_line.c_str());

            return;
        }

        fclose(f);
        control->PrintConfig(temp_line.c_str());

        return;
    }

    if (cmd->FindString("-writelang", temp_line, true) || cmd->FindString("-wl", temp_line, true))
    {
        /**
         * No modo seguro, não permita que um novo arquivo de
         * idioma seja criado. Quem sabe que tipo de arquivo
         * substituiríamos.
         */
        if (control->SecureMode())
        {
            WriteOut(MSG_Get("PROGRAM_CONFIG_SECURE_DISALLOW"));
            return;
        }

        f = fopen(temp_line.c_str(), "wb+");

        if (!f)
        {
            WriteOut(MSG_Get("PROGRAM_CONFIG_FILE_ERROR"), temp_line.c_str());
            return;
        }

        fclose(f);
        MSG_Write(temp_line.c_str());

        return;
    }

    /**
     * Código para mudar para o modo seguro.
     */
    if (cmd->FindExist("-securemode", true))
    {
        control->SwitchToSecureMode();
        WriteOut(MSG_Get("PROGRAM_CONFIG_SECURE_ON"));

        return;
    }

    /**
     * Código para obter a configuração atual.
     * Formato oficial: config -get "propriedade da seção".
     * Como bônus, ele definirá %CONFIG% para esse valor
     * também.
     */
    if (cmd->FindString("-get", temp_line, true))
    {
        std::string temp2 = "";

        /**
         * Então -get n1 n2= pode ser usado sem aspas.
         */
        cmd->GetStringRemain(temp2);

        if (temp2 != "")
        {
            temp_line = temp_line + " " + temp2;
        }

        std::string::size_type space = temp_line.find(" ");

        if (space == std::string::npos)
        {
            WriteOut(MSG_Get("PROGRAM_CONFIG_GET_SYNTAX"));

            return;
        }

        /**
         * Copie a propriedade encontrada para uma nova string
         * e apague do template (cuidado com o espaço).
         */
        std::string prop = temp_line.substr(space + 1);
        temp_line.erase(space);
        Section* sec = control->GetSection(temp_line.c_str());

        if (!sec)
        {
            WriteOut(MSG_Get("PROGRAM_CONFIG_SECTION_ERROR"), temp_line.c_str());
            return;
        }

        std::string val = sec->GetPropValue(prop.c_str());

        if (val == NO_SUCH_PROPERTY)
        {
            WriteOut(MSG_Get("PROGRAM_CONFIG_NO_PROPERTY"), prop.c_str(), temp_line.c_str());
            return;
        }

        WriteOut("%s", val.c_str());
        first_shell->SetEnv("CONFIG", val.c_str());

        return;
    }

    /**
     * Código para as alterações de configuração.
     * Formato oficial: config -set "section property=value".
     * Aceito: sem aspas e/ou sem -set e/ou sem seção e/ou o "="
     * substituído por um " ".
     */

    if (cmd->FindString("-set", temp_line, true))
    {
        /**
         * Obter todos os argumentos.
         */
        std::string temp2 = "";

        /**
         * Então -set n1 n2=n3 pode ser usado sem aspas.
         */
        cmd->GetStringRemain(temp2);

        if (temp2 != "")
        {
            temp_line = temp_line + " " + temp2;
        }
    } else if(!cmd->GetStringRemain(temp_line))
    {
        /**
         * Nenhum conjunto.
         * E nenhum argumento especificado.
         */
        WriteOut(MSG_Get("PROGRAM_CONFIG_USAGE"));
        return;
    };

    /**
     * Entrada desejada: n1 n2=n3.
     */
    char copy[1024];
    strcpy(copy, temp_line.c_str());

    /**
     * Seção separada da propriedade.
     */
    const char* temp = strchr(copy, ' ');

    if ((temp && *temp) || (temp = strchr(copy,'=')))
    {
        copy[temp++ - copy] = 0;
    } else
    {
        WriteOut(MSG_Get("PROGRAM_CONFIG_USAGE"));
        return;
    }

    /**
     * Se n1 n2 n3, substitua o último espaço por `=`.
     */
    const char* sign = strchr(temp, '=');

    if (!sign)
    {
        sign = strchr(temp, ' ');

        if (sign)
        {
            copy[sign - copy] = '=';
        } else
        {
            /**
             * Dois (2) itens especificados (sem espaço nem `=` entre n2 e
             * n3 assumem que eles postaram: valor da propriedade.
             * Tente determinar a seção.
             *
             *
             */
            Section* sec = control->GetSectionFromProperty(copy);

            /**
             * Técnica para permitir configuração ems true.
             */
            if (!sec)
            {
                if (control->GetSectionFromProperty(temp))
                {
                    /**
                     * Situação estranha: pular.
                     */
                    return;
                }

                WriteOut(MSG_Get("PROGRAM_CONFIG_PROPERTY_ERROR"), copy);
                return;
            }

            char buffer[1024];

            strcpy(buffer, copy);
            strcat(buffer, "=");
            strcat(buffer, temp);
            sign = strchr(buffer, ' ');

            if (sign)
            {
                buffer[sign - buffer] = '=';
            }

            strcpy(copy, sec->GetName());
            temp = buffer;
        }
    }

    /**
     * Entrada processada.
     * Agora, a cópia de início do trabalho real contém o
     * provável temporário "sectionname" contém "property=value",
     * a seção é removida e uma nova linha de entrada é fornecida
     * ao analisador de configuração. Em seguida, a seção é
     * reiniciada.
     */
    char* inputline = const_cast<char*>(temp);

    Section* sec = 0;
    sec = control->GetSection(copy);

    if (!sec)
    {
        WriteOut(MSG_Get("PROGRAM_CONFIG_SECTION_ERROR"), copy);
        return;
    }

    sec->ExecuteDestroy(false);
    sec->HandleInputline(inputline);
    sec->ExecuteInit(false);

    return;
}

/**
 *
 */
static void CONFIG_ProgramStart(Program * * make)
{
    *make = new CONFIG;
}

/**
 * Section* sec.
 */
void PROGRAMS_Init(Section*)
{
    /**
     * Configure um retorno de chamada especial para iniciar
     * programas virtuais.
     */
    call_program = CALLBACK_Allocate();

    CALLBACK_Setup(call_program, &PROGRAMS_Handler, CB_RETF, "internal program");
    PROGRAMS_MakeFile("CONFIG.COM", CONFIG_ProgramStart);

    MSG_Add("PROGRAM_CONFIG_FILE_ERROR","Can't open file %s\n");
    MSG_Add("PROGRAM_CONFIG_USAGE","Config tool:\nUse -writeconf filename to write the current config.\nUse -writelang filename to write the current language strings.\n");
    MSG_Add("PROGRAM_CONFIG_SECURE_ON","Switched to secure mode.\n");
    MSG_Add("PROGRAM_CONFIG_SECURE_DISALLOW","This operation is not permitted in secure mode.\n");
    MSG_Add("PROGRAM_CONFIG_SECTION_ERROR","Section %s doesn't exist.\n");
    MSG_Add("PROGRAM_CONFIG_PROPERTY_ERROR","No such section or property.\n");
    MSG_Add("PROGRAM_CONFIG_NO_PROPERTY","There is no property %s in section %s.\n");
    MSG_Add("PROGRAM_CONFIG_GET_SYNTAX","Correct syntax: config -get \"section property\".\n");
}
