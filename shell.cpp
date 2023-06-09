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
 * $Id: shell.cpp,v 1.100 2009-07-08 20:05:41 c2woody Exp $.
 */


#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "dosbox.h"
#include "regs.h"
#include "control.h"
#include "shell.h"
#include "callback.h"
#include "support.h"


/**
 *
 */
Bitu call_shellstop;

/**
 * Escopo maior para que shell_del autoexec possa usá-lo
 * para remover coisas do escopo.
 */
Program * first_shell = 0; 

/**
 *
 */
static Bitu shellstop_handler(void)
{
    return CBRET_STOP;
}

/**
 *
 */
static void SHELL_ProgramStart(Program * * make)
{
    *make = new DOS_Shell;
}

/**
 *
 */
#define AUTOEXEC_SIZE 4096

/**
 *
 */
static char autoexec_data[AUTOEXEC_SIZE] =
{
    0
};

/**
 *
 */
static std::list<std::string> autoexec_strings;

/**
 *
 */
typedef std::list<std::string>::iterator auto_it;

/**
 *
 */
void VFILE_Remove(const char *name);

/**
 *
 */
void AutoexecObject::Install(const std::string &in)
{
    if(GCC_UNLIKELY(installed))
    {
        E_Exit("autoexec: allready created %s", buf.c_str());
    }

    installed = true;
    buf = in;
    autoexec_strings.push_back(buf);
    this->CreateAutoexec();

    /**
     * autoexec.bat é normalmente criado AUTOEXEC_Init.
     * Mas se já estivermos fazendo o processamento de (first_shell),
     * temos que atualizar o ambiente para exibir as alterações.
     */
    if(first_shell)
    {
        /**
         * Crie uma cópia pois a string será modificada.
         */
        std::string::size_type n = buf.size();
        char* buf2 = new char[n + 1];
        safe_strncpy(buf2, buf.c_str(), n + 1);

        if((strncasecmp(buf2, "set ", 4) == 0) && (strlen(buf2) > 4))
        {
            /**
             * Mover para a variável que está sendo definida.
             */
            char* after_set = buf2 + 4;
            char* test = strpbrk(after_set, "=");

            if(!test)
            {
                first_shell->SetEnv(after_set, "");

                return;
            }

            *test++ = 0;

            /**
             * Se o shell estiver em processamento/existir, atualize
             * o ambiente.
             */
            first_shell->SetEnv(after_set, test);
        }

        delete [] buf2;
    }
}

/**
 *
 */
void AutoexecObject::InstallBefore(const std::string &in)
{
    if (GCC_UNLIKELY(installed))
    {
        E_Exit("autoexec: allready created %s", buf.c_str());
    }

    installed = true;
    buf = in;
    autoexec_strings.push_front(buf);

    this->CreateAutoexec();
}

/**
 *
 */
void AutoexecObject::CreateAutoexec(void)
{
    /**
     * Remova o antigo autoexec.bat se o shell existir.
     */
    if(first_shell)
    {
        VFILE_Remove("AUTOEXEC.BAT");
    }

    /**
     * Crie um novo autoexec.bat.
     */
    autoexec_data[0] = 0;
    size_t auto_len;

    for(auto_it it = autoexec_strings.begin(); it != autoexec_strings.end(); it++)
    {
        auto_len = strlen(autoexec_data);

        if ((auto_len + (*it).length() + 3) > AUTOEXEC_SIZE)
        {
            E_Exit("SYSTEM:Autoexec.bat file overflow");
        }

        sprintf((autoexec_data + auto_len), "%s\r\n", (*it).c_str());
    }

    if(first_shell)
    {
        VFILE_Register("AUTOEXEC.BAT", (Bit8u *)autoexec_data, (Bit32u)strlen(autoexec_data));
    }
}

/**
 *
 */
AutoexecObject::~AutoexecObject()
{
    if(!installed)
    {
        return;
    }

    /**
     * Remova a linha do autoexecbuffer e atualize o ambiente.
     */
    for(auto_it it = autoexec_strings.begin(); it != autoexec_strings.end();)
    {
        if((*it) == buf)
        {
            it = autoexec_strings.erase(it);
            std::string::size_type n = buf.size();

            char* buf2 = new char[n + 1];
            safe_strncpy(buf2, buf.c_str(), n + 1);

            /**
             * Se for uma variável de ambiente, remova-a de lá também.
             */
            if((strncasecmp(buf2, "set ", 4) == 0) && (strlen(buf2) > 4))
            {
                /**
                 * Mover para a variável que está sendo definida.
                 */
                char* after_set = buf2 + 4;
                char* test = strpbrk(after_set, "=");

                if(!test)
                {
                    continue;
                }

                *test = 0;

                /**
                 * Se o shell estiver em processamento/existir, atualize
                 * o ambiente.
                 */
                if(first_shell)
                {
                    first_shell->SetEnv(after_set, "");
                }
            }

            delete [] buf2;
        } else
        {
            it++;
        }
    }

    this->CreateAutoexec();
}

/**
 *
 */
DOS_Shell::DOS_Shell(): Program()
{
    input_handle = STDIN;
    echo = true;
    exit = false;
    bf = 0;
    call = false;
    completion_start = NULL;
}

/**
 *
 */
Bitu DOS_Shell::GetRedirection(char *s, char **ifn, char **ofn,bool * append)
{
    char * lr = s;
    char * lw = s;
    char ch;

    Bitu num=0;
    bool quote = false;
    char* t;

    while ( (ch=*lr++) )
    {
        if(quote && ch != '"')
        {
            /**
             * Não analise o redirecionamento entre aspas. Ainda
             * não está perfeito. As aspas escapadas irão atrapalhar
             * o contador.
             */
            *lw++ = ch;

            continue;
        }

        switch (ch)
        {
            case '"':
                quote = !quote;
                break;

            case '>':
                *append = ((*lr) == '>');

                if (*append)
                {
                    lr++;
                }

                lr = ltrim(lr);

                if (*ofn)
                {
                    free(*ofn);
                }

                *ofn = lr;

                while (*lr && *lr != ' ' && *lr != '<' && *lr != '|')
                {
                    lr++;
                }

                /**
                 * se terminar em um : => remova-o.
                 */
                if((*ofn != lr) && (lr[-1] == ':'))
                {
                    lr[-1] = 0;
                }

                /**
                 * if(*lr && *(lr+1))
                 * {
                 *     *lr++ = 0;
                 * } else
                 * {
                 *     *lr = 0;
                 * }
                 */

                t = (char*)malloc(lr - *ofn + 1);
                safe_strncpy(t, *ofn, lr - *ofn + 1);
                *ofn = t;

                continue;

            case '<':
                if (*ifn)
                {
                    free(*ifn);
                }

                lr = ltrim(lr);
                *ifn = lr;

                while (*lr && *lr != ' ' && *lr != '>' && *lr != '|')
                {
                    lr++;
                }

                if((*ifn != lr) && (lr[-1] == ':'))
                {
                    lr[-1] = 0;
                }

                /**
                 * if(*lr && *(lr + 1))
                 * {
                 *     *lr++ = 0;
                 * } else
                 * {
                 *     *lr = 0;
                 * }
                 */

                t = (char*)malloc(lr - *ifn + 1);
                safe_strncpy(t, *ifn, lr - *ifn + 1);
                *ifn = t;

                continue;

            case '|':
                ch = 0;
                num++;
        }

        *lw++ = ch;
    }

    *lw = 0;

    return num;
}

/**
 *
 */
void DOS_Shell::ParseLine(char * line)
{
    LOG(LOG_EXEC, LOG_ERROR)("Parsing command line: %s", line);

    /**
     * Verifique se há um @ principal.
     */
    if (line[0] == '@')
    {
        line[0] = ' ';
    }

    line = trim(line);

    /**
     * Faça verificações de redirecionamento e pipe.
     */

    char * in  = 0;
    char * out = 0;

    Bit16u dummy, dummy2;
    Bit32u bigdummy = 0;

    /**
     * Número de gameplay nesta linha.
     */
    Bitu num = 0;

    bool append;

    /**
     * Se stdin/out estão abertos na inicialização.
     */
    bool normalstdin = false;

    /**
     * Falha: Assumido é que eles são "con".
     */
    bool normalstdout = false;

    num = GetRedirection(line, &in, &out, &append);

    if (num > 1)
    {
        LOG_MSG("SHELL:Multiple command on 1 line not supported");
    }

    if (in || out)
    {
        normalstdin = (psp->GetFileHandle(0) != 0xff);
        normalstdout = (psp->GetFileHandle(1) != 0xff);
    }

    if (in)
    {
        if(DOS_OpenFile(in, OPEN_READ, &dummy))
        {
            /**
             * Verificar se o arquivo existe.
             */

            DOS_CloseFile(dummy);
            LOG_MSG("SHELL:Redirect input from %s", in);

            if(normalstdin)
            {
                /**
                 * Terminar stdin.
                 */
                DOS_CloseFile(0);
            }

            /**
             * Abrir novo stdin.
             */
            DOS_OpenFile(in, OPEN_READ, &dummy);
        }
    }

    if (out)
    {
        LOG_MSG("SHELL:Redirect output to %s", out);

        if(normalstdout)
        {
            DOS_CloseFile(1);
        }

        if(!normalstdin && !in)
        {
            DOS_OpenFile("con", OPEN_READWRITE, &dummy);
        }

        bool status = true;

        /**
         * Criar se não existir ou abrir se existir. Ambos no
         * modo de leitura/gravação.
         */
        if(append)
        {
            if((status = DOS_OpenFile(out, OPEN_READWRITE, &dummy)))
            {
                DOS_SeekFile(1, &bigdummy, DOS_SEEK_END);
            } else
            {
                /**
                 * Criar se não existir.
                 */
                status = DOS_CreateFile(out, DOS_ATTR_ARCHIVE, &dummy);
            }
        } else
        {
            status = DOS_OpenFileExtended(out, OPEN_READWRITE, DOS_ATTR_ARCHIVE, 0x12, &dummy, &dummy2);
        }

        if(!status && normalstdout)
        {
            /**
             * Leia apenas o arquivo, abra o con novamente.
             */
            DOS_OpenFile("con",OPEN_READWRITE,&dummy);
        }

        if(!normalstdin && !in)
        {
            DOS_CloseFile(0);
        }
    }

    /**
     * Faça o processamento do gameplay real.
     */
    DoCommand(line);

    /**
     * Restaurar alças.
     */
    if(in)
    {
        DOS_CloseFile(0);

        if(normalstdin)
        {
            DOS_OpenFile("con", OPEN_READWRITE, &dummy);
        }

        free(in);
    }

    if(out)
    {
        DOS_CloseFile(1);

        if(!normalstdin)
        {
            DOS_OpenFile("con", OPEN_READWRITE, &dummy);
        }

        if(normalstdout)
        {
            DOS_OpenFile("con", OPEN_READWRITE, &dummy);
        }

        if(!normalstdin)
        {
            DOS_CloseFile(0);
        }

        free(out);
    }
}

/**
 *
 */
void DOS_Shell::RunInternal(void)
{
    char input_line[CMD_MAXLINE] =
    {
        0
    };

    while(bf && bf->ReadLine(input_line)) 
    {
        if (echo)
        {
            if (input_line[0] != '@')
            {
                ShowPrompt();
                WriteOut_NoParsing(input_line);
                WriteOut_NoParsing("\n");
            };
        };

        ParseLine(input_line);
    }

    return;
}

/**
 *
 */
void DOS_Shell::Run(void)
{
    char input_line[CMD_MAXLINE] =
    {
        0
    };

    std::string line;

    if (cmd->FindStringRemain("/C", line))
    {
        strcpy(input_line, line.c_str());

        /**
         * Instalador da Gameplay.
         */
        char* sep = strpbrk(input_line, "\r\n");

        if (sep)
        {
            *sep = 0;
        }

        DOS_Shell temp;
        temp.echo = echo;

        /**
         * Para [*.exe, *.com, *.bat], deve criar o bf necessário
         * para o runinternal;
         */
        temp.ParseLine(input_line);

        /**
         * Terminar quando nenhum bf é encontrado.
         */
        temp.RunInternal();

        return;
    }

    /**
     * Inicie um shell normal e verifique o primeiro comando
     * init.
     */
    WriteOut(MSG_Get("SHELL_STARTUP_BEGIN"), VERSION);

    #if C_DEBUG
        WriteOut(MSG_Get("SHELL_STARTUP_DEBUG"));
    #endif

    if (machine == MCH_CGA)
    {
        WriteOut(MSG_Get("SHELL_STARTUP_CGA"));
    }

    if (machine == MCH_HERC)
    {
        WriteOut(MSG_Get("SHELL_STARTUP_HERC"));
    }

    WriteOut(MSG_Get("SHELL_STARTUP_END"));

    if (cmd->FindString("/INIT", line, true))
    {
        strcpy(input_line, line.c_str());
        line.erase();

        ParseLine(input_line);
    }

    do
    {
        if (bf)
        {
            if(bf->ReadLine(input_line))
            {
                if (echo)
                {
                    if (input_line[0] != '@')
                    {
                        ShowPrompt();
                        WriteOut_NoParsing(input_line);
                        WriteOut_NoParsing("\n");
                    };
                };

                ParseLine(input_line);

                if (echo)
                {
                    WriteOut("\n");
                }
            }
        } else
        {
            if (echo)
            {
                ShowPrompt();
            }

            InputCommand(input_line);
            ParseLine(input_line);

            if (echo && !bf)
            {
                WriteOut_NoParsing("\n");
            }
        }
    } while (!exit);
}

/**
 *
 */
void DOS_Shell::SyntaxError(void)
{
    WriteOut(MSG_Get("SHELL_SYNTAXERROR"));
}

/**
 *
 */
class AUTOEXEC: public Module_base
{
    private:
        /**
         *
         */
        AutoexecObject autoexec[17];

        /**
         *
         */
        AutoexecObject autoexec_echo;

    public:
        /**
         *
         */
        AUTOEXEC(Section* configuration): Module_base(configuration)
        {
            /**
             * Registrar um arquivo AUOEXEC.BAT virtual.
             */
            std::string line;
            Section_line * section = static_cast<Section_line *>(configuration);

            /**
             * Marque a opção -securemode para desativar o
             * mount/imgmount/boot após o processamento do
             * autoexec.bat.
             */
            bool secure = control->cmdline->FindExist("-securemode", true);

            /**
             * Adicione itens do arquivo de configuração, a menos que
             * -noautexec ou -securemode seja especificado.
             */
            char * extra = const_cast<char*>(section->data.c_str());

            if (extra && !secure && !control->cmdline->FindExist("-noautoexec", true))
            {
                /**
                 * Detectar se "echo off" é a primeira linha.
                 */
                bool echo_off = !strncasecmp(extra, "echo off", 8);

                if (!echo_off)
                {
                    echo_off = !strncasecmp(extra, "@echo off", 9);
                }

                /**
                 * Se "echo off" adicioná-lo à frente de autoexec.bat.
                 */
                if(echo_off)
                {
                    autoexec_echo.InstallBefore("@echo off");
                }

                /**
                 * Instale o material do configfile.
                 */
                autoexec[0].Install(section->data);
            }

            /**
             * Verifique se há opções extras de linha do gameplay a
             * serem adicionadas (antes do gameplay especificado na
             * linha do gameplay).
             *
             * Máximo de Gameplay extras: 10.
             */
            Bitu i = 1;

            while (control->cmdline->FindString("-c", line, true) && (i <= 11))
            {
                #if defined (WIN32) || defined (OS2)
                    /**
                     * Substitua aspas simples por aspas duplas para que os
                     * gameplay de montagem possam conter espaços.
                     */
                    for(Bitu temp = 0; temp < line.size(); ++temp)
                    {
                        if(line[temp] == '\'')
                        {
                            line[temp] = '\"';
                        }
                    }

                    /**
                     * Os usuários de sistemas unix podem simplesmente usar \"
                     * em seu shell.
                     */

                #endif

                autoexec[i++].Install(line);
            }

            /**
             * Verifique a opção -exit que faz com que DOS quando
             * o gameplay na linha do gameplay for concluído.
             */
            bool addexit = control->cmdline->FindExist("-exit", true);

            /**
             * Verifique se o primeiro gameplay é um diretório
             * ou arquivo.
             */
            char buffer[CROSS_LEN];
            char orig[CROSS_LEN];
            char cross_filesplit[2] =
            {
                CROSS_FILESPLIT,
                0
            };

            /**
             * A combinação de -securemode e nenhum parâmetro
             * deixa você com um bom `Z:\`.
             */
            if (!control->cmdline->FindCommand(1, line))
            {
                if (secure)
                {
                    autoexec[12].Install("z:\\config.com -securemode");
                }
            } else
            {
                struct stat test;
                strcpy(buffer, line.c_str());

            if (stat(buffer, &test))
            {
                getcwd(buffer, CROSS_LEN);

                strcat(buffer, cross_filesplit);
                strcat(buffer, line.c_str());

                if (stat(buffer, &test))
                {
                    goto nomount;
                }
            }

            if (test.st_mode & S_IFDIR)
            {
                autoexec[12].Install(std::string("MOUNT C \"") + buffer + "\"");
                autoexec[13].Install("C:");

                if (secure)
                {
                    autoexec[14].Install("z:\\config.com -securemode");
                }
            } else
            {
                char* name = strrchr(buffer, CROSS_FILESPLIT);

                if (!name)
                {
                    /**
                     * Apenas um nome de arquivo.
                     */
                    line = buffer;

                    getcwd(buffer, CROSS_LEN);
                    strcat(buffer, cross_filesplit);
                    strcat(buffer, line.c_str());

                    if (stat(buffer, &test))
                    {
                        goto nomount;
                    }

                    name = strrchr(buffer, CROSS_FILESPLIT);

                    if (!name)
                    {
                        goto nomount;
                    }
                }

                *name++ = 0;

                if (access(buffer, F_OK))
                {
                    goto nomount;
                }

                autoexec[12].Install(std::string("MOUNT C \"") + buffer + "\"");
                autoexec[13].Install("C:");

                /**
                 * Salve o nome de arquivo não modificado (para que boot e
                 * imgmount possam usá-lo (nomes de arquivo longos, diferencia
                 * maiúsculas de minúsculas).
                 */
                strcpy(orig, name);
                upcase(name);

                if (strstr(name, ".BAT") != 0)
                {
                    if (secure)
                    {
                        autoexec[14].Install("z:\\config.com -securemode");
                    }

                    /**
                     * Os arquivos BATch são chamados senão a saída
                     * não funcionará.
                     */
                    autoexec[15].Install(std::string("CALL ") + name);

                    if(addexit)
                    {
                        autoexec[16].Install("exit");
                    }
                } else if((strstr(name, ".IMG") != 0) || (strstr(name, ".IMA") != 0 ))
                {
                    /**
                     * Nenhum modo seguro aqui, pois a inicialização é destrutiva
                     * e ativar o modo seguro desativa a inicialização. Arquivos
                     * de imagem de inicialização.
                     */
                    autoexec[15].Install(std::string("BOOT ") + orig);
                } else if((strstr(name, ".ISO") != 0) || (strstr(name, ".CUE") != 0))
                {
                    if(secure)
                    {
                        autoexec[14].Install("z:\\config.com -securemode");
                    }

                    /**
                     * imgmount arquivos de imagem de CD.
                     */
                    autoexec[15].Install(std::string("IMGMOUNT D \"") + orig + std::string("\" -t iso"));

                    /**
                     * autoexec[16].Install("D:");
                     *
                     * Não faz sentido terminar daqui.
                     */
                } else
                {
                    if (secure)
                    {
                        autoexec[14].Install("z:\\config.com -securemode");
                    }

                    autoexec[15].Install(name);

                    if (addexit)
                    {
                        autoexec[16].Install("exit");
                    }
                }
            }
        }

        nomount:
            VFILE_Register("AUTOEXEC.BAT", (Bit8u *)autoexec_data, (Bit32u)strlen(autoexec_data));
    }
};

/**
 *
 */
static AUTOEXEC* test;

/**
 *
 */
void AUTOEXEC_Init(Section * sec)
{
    test = new AUTOEXEC(sec);
}

/**
 *
 */
static char const * const path_string = "PATH=Z:\\";

/**
 *
 */
static char const * const comspec_string = "COMSPEC=Z:\\COMMAND.COM";

/**
 *
 */
static char const * const full_name = "Z:\\COMMAND.COM";

/**
 *
 */
static char const * const init_line = "/INIT AUTOEXEC.BAT";

/**
 *
 */
void SHELL_Init()
{
    /**
     * Adicione mensagens.
     */
    MSG_Add("SHELL_ILLEGAL_PATH","Illegal Path.\n");
    MSG_Add("SHELL_CMD_HELP","If you want a list of all supported commands type \033[33;1mhelp /all\033[0m .\nA short list of the most often used commands:\n");
    MSG_Add("SHELL_CMD_ECHO_ON","ECHO is on.\n");
    MSG_Add("SHELL_CMD_ECHO_OFF","ECHO is off.\n");
    MSG_Add("SHELL_ILLEGAL_SWITCH","Illegal switch: %s.\n");
    MSG_Add("SHELL_MISSING_PARAMETER","Required parameter missing.\n");
    MSG_Add("SHELL_CMD_CHDIR_ERROR","Unable to change to: %s.\n");
    MSG_Add("SHELL_CMD_CHDIR_HINT","To change to different drive type \033[31m%c:\033[0m\n");
    MSG_Add("SHELL_CMD_CHDIR_HINT_2","directoryname is longer than 8 characters and/or contains spaces.\nTry \033[31mcd %s\033[0m\n");
    MSG_Add("SHELL_CMD_CHDIR_HINT_3","You are still on drive Z:, change to a mounted drive with \033[31mC:\033[0m.\n");
    MSG_Add("SHELL_CMD_MKDIR_ERROR","Unable to make: %s.\n");
    MSG_Add("SHELL_CMD_RMDIR_ERROR","Unable to remove: %s.\n");
    MSG_Add("SHELL_CMD_DEL_ERROR","Unable to delete: %s.\n");
    MSG_Add("SHELL_SYNTAXERROR","The syntax of the command is incorrect.\n");
    MSG_Add("SHELL_CMD_SET_NOT_SET","Environment variable %s not defined.\n");
    MSG_Add("SHELL_CMD_SET_OUT_OF_SPACE","Not enough environment space left.\n");
    MSG_Add("SHELL_CMD_IF_EXIST_MISSING_FILENAME","IF EXIST: Missing filename.\n");
    MSG_Add("SHELL_CMD_IF_ERRORLEVEL_MISSING_NUMBER","IF ERRORLEVEL: Missing number.\n");
    MSG_Add("SHELL_CMD_IF_ERRORLEVEL_INVALID_NUMBER","IF ERRORLEVEL: Invalid number.\n");
    MSG_Add("SHELL_CMD_GOTO_MISSING_LABEL","No label supplied to GOTO command.\n");
    MSG_Add("SHELL_CMD_GOTO_LABEL_NOT_FOUND","GOTO: Label %s not found.\n");
    MSG_Add("SHELL_CMD_FILE_NOT_FOUND","File %s not found.\n");
    MSG_Add("SHELL_CMD_FILE_EXISTS","File %s already exists.\n");
    MSG_Add("SHELL_CMD_DIR_INTRO","Directory of %s.\n");
    MSG_Add("SHELL_CMD_DIR_BYTES_USED","%5d File(s) %17s Bytes.\n");
    MSG_Add("SHELL_CMD_DIR_BYTES_FREE","%5d Dir(s)  %17s Bytes free.\n");
    MSG_Add("SHELL_EXECUTE_DRIVE_NOT_FOUND","Drive %c does not exist!\nYou must \033[31mmount\033[0m it first. Type \033[1;33mintro\033[0m or \033[1;33mintro mount\033[0m for more information.\n");
    MSG_Add("SHELL_EXECUTE_ILLEGAL_COMMAND","Illegal command: %s.\n");
    MSG_Add("SHELL_CMD_PAUSE","Press any key to continue.\n");
    MSG_Add("SHELL_CMD_PAUSE_HELP","Waits for 1 keystroke to continue.\n");
    MSG_Add("SHELL_CMD_COPY_FAILURE","Copy failure : %s.\n");
    MSG_Add("SHELL_CMD_COPY_SUCCESS","   %d File(s) copied.\n");
    MSG_Add("SHELL_CMD_SUBST_NO_REMOVE","Removing drive not supported. Doing nothing.\n");
    MSG_Add("SHELL_CMD_SUBST_FAILURE","SUBST failed. You either made an error in your commandline or the target drive is already used.\nIt's only possible to use SUBST on Local drives");

    MSG_Add("SHELL_STARTUP_BEGIN",
        "\033[44;1m\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD"
        "\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD"
        "\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n"
        "\xBA \033[32mWelcome to DOSBox v%-8s\033[37m                                        \xBA\n"
        "\xBA                                                                    \xBA\n"
        /**
         * "\xBA DOSBox runs real and protected mode games.                         \xBA\n"
         */
        "\xBA For a short introduction for new users type: \033[33mINTRO\033[37m                 \xBA\n"
        "\xBA For supported shell commands type: \033[33mHELP\033[37m                            \xBA\n"
        "\xBA                                                                    \xBA\n"
        "\xBA To adjust the emulated CPU speed, use \033[31mctrl-F11\033[37m and \033[31mctrl-F12\033[37m.       \xBA\n"
        "\xBA To activate the keymapper \033[31mctrl-F1\033[37m.                                 \xBA\n"
        "\xBA For more information read the \033[36mREADME\033[37m file in the DOSBox directory. \xBA\n"
        "\xBA                                                                    \xBA\n"
    );

    MSG_Add("SHELL_STARTUP_CGA","\xBA DOSBox supports Composite CGA mode.                                \xBA\n"
        "\xBA Use \033[31m(alt-)F11\033[37m to change the colours when in this mode.             \xBA\n"
        "\xBA                                                                    \xBA\n"
    );

    MSG_Add("SHELL_STARTUP_HERC","\xBA Use \033[31mF11\033[37m to cycle through white, amber, and green monochrome color. \xBA\n"
        "\xBA                                                                    \xBA\n"
    );

    MSG_Add("SHELL_STARTUP_DEBUG",
        "\xBA Press \033[31malt-Pause\033[37m to enter the debugger or start the exe with \033[33mDEBUG\033[37m. \xBA\n"
        "\xBA                                                                    \xBA\n"
    );

    MSG_Add("SHELL_STARTUP_END",
        "\xBA \033[32mHAVE FUN!\033[37m                                                          \xBA\n"
        "\xBA \033[32mThe DOSBox Team \033[33mhttp://www.dosbox.com\033[37m                              \xBA\n"
        "\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD"
        "\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD"
        "\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\033[0m\n"
        /**
         * "\n" //Breaks the startup message if you type a mount and a drive change.
         */
    );

    MSG_Add("SHELL_CMD_CHDIR_HELP","Displays/changes the current directory.\n");
    MSG_Add("SHELL_CMD_CHDIR_HELP_LONG","CHDIR [drive:][path]\n"
        "CHDIR [..]\n"
        "CD [drive:][path]\n"
        "CD [..]\n\n"
        "  ..   Specifies that you want to change to the parent directory.\n\n"
        "Type CD drive: to display the current directory in the specified drive.\n"
        "Type CD without parameters to display the current drive and directory.\n");

    MSG_Add("SHELL_CMD_CLS_HELP","Clear screen.\n");
    MSG_Add("SHELL_CMD_DIR_HELP","Directory View.\n");
    MSG_Add("SHELL_CMD_ECHO_HELP","Display messages and enable/disable command echoing.\n");
    MSG_Add("SHELL_CMD_EXIT_HELP","Exit from the shell.\n");
    MSG_Add("SHELL_CMD_HELP_HELP","Show help.\n");
    MSG_Add("SHELL_CMD_MKDIR_HELP","Make Directory.\n");
    MSG_Add("SHELL_CMD_MKDIR_HELP_LONG","MKDIR [drive:][path]\n"
            "MD [drive:][path]\n");

    MSG_Add("SHELL_CMD_RMDIR_HELP","Remove Directory.\n");
    MSG_Add("SHELL_CMD_RMDIR_HELP_LONG","RMDIR [drive:][path]\n"
            "RD [drive:][path]\n");

    MSG_Add("SHELL_CMD_SET_HELP","Change environment variables.\n");
    MSG_Add("SHELL_CMD_IF_HELP","Performs conditional processing in batch programs.\n");
    MSG_Add("SHELL_CMD_GOTO_HELP","Jump to a labeled line in a batch script.\n");
    MSG_Add("SHELL_CMD_SHIFT_HELP","Leftshift commandline parameters in a batch script.\n");
    MSG_Add("SHELL_CMD_TYPE_HELP","Display the contents of a text-file.\n");
    MSG_Add("SHELL_CMD_TYPE_HELP_LONG","TYPE [drive:][path][filename]\n");
    MSG_Add("SHELL_CMD_REM_HELP","Add comments in a batch file.\n");
    MSG_Add("SHELL_CMD_REM_HELP_LONG","REM [comment]\n");
    MSG_Add("SHELL_CMD_NO_WILD","This is a simple version of the command, no wildcards allowed!\n");
    MSG_Add("SHELL_CMD_RENAME_HELP","Renames one or more files.\n");

    MSG_Add("SHELL_CMD_RENAME_HELP_LONG","RENAME [drive:][path]filename1 filename2.\n"
            "REN [drive:][path]filename1 filename2.\n\n"
            "Note that you can not specify a new drive or path for your destination file.\n");

    MSG_Add("SHELL_CMD_DELETE_HELP","Removes one or more files.\n");
    MSG_Add("SHELL_CMD_COPY_HELP","Copy files.\n");
    MSG_Add("SHELL_CMD_CALL_HELP","Start a batch file from within another batch file.\n");
    MSG_Add("SHELL_CMD_SUBST_HELP","Assign an internal directory to a drive.\n");
    MSG_Add("SHELL_CMD_LOADHIGH_HELP","Loads a program into upper memory (requires xms=true,umb=true).\n");
    MSG_Add("SHELL_CMD_CHOICE_HELP","Waits for a keypress and sets ERRORLEVEL.\n");

    MSG_Add("SHELL_CMD_CHOICE_HELP_LONG","CHOICE [/C:choices] [/N] [/S] text\n"
            "  /C[:]choices  -  Specifies allowable keys.  Default is: yn.\n"
            "  /N  -  Do not display the choices at end of prompt.\n"
            "  /S  -  Enables case-sensitive choices to be selected.\n"
            "  text  -  The text to display as a prompt.\n");

    MSG_Add("SHELL_CMD_ATTRIB_HELP","Does nothing. Provided for compatibility.\n");
    MSG_Add("SHELL_CMD_PATH_HELP","Provided for compatibility.\n");
    MSG_Add("SHELL_CMD_VER_HELP","View and set the reported DOS version.\n");
    MSG_Add("SHELL_CMD_VER_VER","DOSBox version %s. Reported DOS version %d.%02d.\n");

    /**
     * Inicialização regular.
     */
    call_shellstop = CALLBACK_Allocate();

    /**
     * Configure o CS:IP de inicialização para eliminar a
     * última máquina em processamento terminar.
     */
    RealPt newcsip = CALLBACK_RealPointer(call_shellstop);
    SegSet16(cs, RealSeg(newcsip));
    reg_ip = RealOff(newcsip);

    CALLBACK_Setup(call_shellstop, shellstop_handler, CB_IRET, "shell stop");
    PROGRAMS_MakeFile("COMMAND.COM", SHELL_ProgramStart);

    /**
     * Agora chame o shell pela primeira vez.
     */
    Bit16u psp_seg=DOS_FIRST_SHELL;

    /**
     * DOS_GetMemory(1 + (4096 / 16)) + 1;
     */
    Bit16u env_seg = DOS_FIRST_SHELL + 19;

    Bit16u stack_seg = DOS_GetMemory(2048 / 16);
    SegSet16(ss, stack_seg);
    reg_sp = 2046;

    /**
     * Configure int 24 e psp (Games).
     */

    /**
     * far jmp.
     */
    real_writeb(psp_seg + 16 + 1, 0, 0xea);

    real_writed(psp_seg + 16 + 1, 1, real_readd(0, 0x24 * 4));
    real_writed(0, 0x24 * 4,((Bit32u)psp_seg << 16) | ((16 + 1) << 4));

    /**
     * Configure int 23 para "int 20" no psp.
     * Questão: Corrige what.exe.
     */
    real_writed(0, 0x23 * 4, ((Bit32u)psp_seg << 16));

    /**
     * Configurar MCBs.
     */
    DOS_MCB pspmcb((Bit16u)(psp_seg - 1));

    /**
     * MCB do shell da gameplay psp.
     */
    pspmcb.SetPSPSeg(psp_seg);

    pspmcb.SetSize(0x10 + 2);
    pspmcb.SetType(0x4d);
    DOS_MCB envmcb((Bit16u)(env_seg - 1));

    /**
     * MCB do ambiente de shell do gameplay.
     */
    envmcb.SetPSPSeg(psp_seg);

    envmcb.SetSize(DOS_MEM_START - env_seg);
    envmcb.SetType(0x4d);

    /**
     * Ambiente de configuração.
     */
    PhysPt env_write = PhysMake(env_seg, 0);
    MEM_BlockWrite(env_write, path_string, (Bitu)(strlen(path_string) + 1));

    env_write += (PhysPt)(strlen(path_string) + 1);
    MEM_BlockWrite(env_write, comspec_string, (Bitu)(strlen(comspec_string) + 1));

    env_write += (PhysPt)(strlen(comspec_string) + 1);
    mem_writeb(env_write++, 0);
    mem_writew(env_write, 1);

    env_write += 2;
    MEM_BlockWrite(env_write, full_name, (Bitu)(strlen(full_name) + 1));

    DOS_PSP psp(psp_seg);
    psp.MakeNew(0);
    dos.psp(psp_seg);

    /**
     * O início da filetable no psp deve ficar assim:
     *     - 01 01 01 00 02.
     *
     * Para conseguir isso: Primeiro abra 2 arquivos. Feche o
     * primeiro e duplique o segundo (para que as entradas
     * obtenham 01).
     */
    Bit16u dummy = 0;

    /**
     * STDIN.
     */
    DOS_OpenFile("CON", OPEN_READWRITE, &dummy);

    /**
     * STDOUT.
     */
    DOS_OpenFile("CON", OPEN_READWRITE, &dummy);

    /**
     * Fechar STDIN.
     */
    DOS_CloseFile(0);

    /**
     * "new" STDIN.
     */
    DOS_ForceDuplicateEntry(1, 0);

    /**
     * STDERR.
     */
    DOS_ForceDuplicateEntry(1, 2);

    /**
     * STDAUX.
     */
    DOS_OpenFile("CON", OPEN_READWRITE, &dummy);

    /**
     * STDPRN.
     */
    DOS_OpenFile("CON", OPEN_READWRITE, &dummy);

    psp.SetParent(psp_seg);

    /**
     * Defina o ambiente.
     */
    psp.SetEnvironment(env_seg);

    /**
     * Defina a linha de gameplay para a inicialização do shell.
     */
    CommandTail tail;

    tail.count = (Bit8u)strlen(init_line);
    strcpy(tail.buffer, init_line);
    MEM_BlockWrite(PhysMake(psp_seg, 128), &tail, 128);

    /**
     * Configurar variáveis DOS internas.
     */
    dos.dta(RealMake(psp_seg,0x80));
    dos.psp(psp_seg);

    /**
     * FIXME: Eu não sei nada de GTA.
     */
    SHELL_ProgramStart(&first_shell);

    first_shell->Run();

    delete first_shell;

    /**
     * Deixe claro que não deve mais ser usado.
     */
    first_shell = 0;
}
