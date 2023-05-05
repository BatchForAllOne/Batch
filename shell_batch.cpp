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

#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "support.h"


/**
 * $Id: shell_batch.cpp,v 1.36 2009-07-03 19:36:56 qbix79 Exp $.
 */


/**
 *
 */
BatchFile::BatchFile(DOS_Shell * host,char const * const resolved_name,char const * const entered_name, char const * const cmd_line)
{
    location = 0;
    prev = host->bf;
    echo = host->echo;
    shell = host;
    char totalname[DOS_PATHLENGTH + 4];

    /**
     * Obtenha o nome completo, incluindo a especificação
     * da unidade.
     */
    DOS_Canonicalize(resolved_name, totalname);

    cmd = new CommandLine(entered_name,cmd_line);
    filename = totalname;

    /**
     * Teste se o arquivo pode ser aberto.
     */
    if (!DOS_OpenFile(totalname, (DOS_NOT_INHERIT | OPEN_READ), &file_handle))
    {
        /**
         * TODO - Pense em algo melhor.
         */
        E_Exit("SHELL:Can't open BatchFile %s", totalname);
    }

    DOS_CloseFile(file_handle);
}

/**
 *
 */
BatchFile::~BatchFile()
{
    delete cmd;

    shell->bf = prev;
    shell->echo = echo;
}

/**
 *
 */
bool BatchFile::ReadLine(char * line)
{
    /**
     * Abra o batchfile e procure a posição armazenada.
     */
    if (!DOS_OpenFile(filename.c_str(), (DOS_NOT_INHERIT | OPEN_READ), &file_handle))
    {
        LOG(LOG_MISC,LOG_ERROR)("ReadLine Can't open BatchFile %s", filename.c_str());

        delete this;
        return false;
    }

    DOS_SeekFile(file_handle, &(this->location), DOS_SEEK_SET);
    Bit8u c = 0;
    Bit16u n = 1;

    char temp[CMD_MAXLINE];

    emptyline:
        char * cmd_write = temp;

        do
        {
            n = 1;
            DOS_ReadFile(file_handle, &c, &n);

            if (n > 0)
            {
                /**
                 * Por que estamos filtrando isso ?
                 * Lista de exclusão: guia para arquivos em lote escape
                 * para ansi backspace para alien odyssey.
                 */
                if (c > 31 || c == 0x1b || c == '\t' || c == 8)
                {
                    /**
                     * Adicione-o apenas se houver espaço para ele (e à
                     * direita de zero) no buffer, mas faça a verificação
                     * aqui no final. Então continuamos lendo até EOL/EOF.
                     */
                    if (((cmd_write - temp) + 1) < (CMD_MAXLINE - 1))
                    {
                        *cmd_write++ = c;
                    }
                }
            }
        } while (c != '\n' && n);

        *cmd_write = 0;

        if (!n && cmd_write == temp)
        {
            /**
             * Feche o arquivo e exclua o arquivo bat.
             */
            DOS_CloseFile(file_handle);
            delete this;

            return false;
        }

        if (!strlen(temp))
        {
            goto emptyline;
        }

        if (temp[0] == ':')
        {
            goto emptyline;
        }

        /**
         * Agora analise a linha lida do arquivo bat para % stuff.
         */
        cmd_write = line;

        /**
         *
         */
        char * cmd_read = temp;

        while (*cmd_read)
        {
            if (*cmd_read == '%')
            {
                cmd_read++;

                if (cmd_read[0] == '%')
                {
                    cmd_read++;

                    if (((cmd_write - line) + 1) < (CMD_MAXLINE - 1))
                    {
                        *cmd_write++ = '%';
                    }

                    continue;
                }

                if (cmd_read[0] == '0')
                {
                    /**
                     * Corrião %0.
                     */
                    const char *file_name = cmd->GetFileName();

                    cmd_read++;
                    size_t name_len = strlen(file_name);

                    if (((cmd_write - line) + name_len) < (CMD_MAXLINE - 1))
                    {
                        strcpy(cmd_write,file_name);
                        cmd_write += name_len;
                    }

                    continue;
                }

                char next = cmd_read[0];

                if(next > '0' && next <= '9')
                {
                    /**
                     * Corrião %1 %2 .. %9. - Leitor de progresso.
                     */
                    cmd_read++;
                    next -= '0';

                    if (cmd->GetCount() < (unsigned int)next)
                    {
                        continue;
                    }

                    std::string word;

                    if (!cmd->FindCommand(next, word))
                    {
                        continue;
                    }

                    size_t name_len = strlen(word.c_str());

                    if (((cmd_write - line) + name_len) < (CMD_MAXLINE - 1))
                    {
                        strcpy(cmd_write, word.c_str());
                        cmd_write += name_len;
                    }

                    continue;
                } else
                {
                    /**
                     * Não é um número de linha de gameplay, tem que ser um ambiente.
                     */
                    char * first = strchr(cmd_read,'%');

                    /**
                     * Afinal, não há env. Ignorar um único %.
                     */
                    if (!first)
                    {
                        /**
                         * *cmd_write++ = '%';
                         */
                        continue;
                    }

                    *first++ = 0;
                    std::string env;

                    if (shell->GetEnvStr(cmd_read, env))
                    {
                        const char* equals = strchr(env.c_str(), '=');

                        if (!equals)
                        {
                            continue;
                        }

                        equals++;
                        size_t name_len = strlen(equals);

                        if (((cmd_write - line) + name_len) < (CMD_MAXLINE - 1))
                        {
                            strcpy(cmd_write, equals);
                            cmd_write += name_len;
                        }
                    }

                    cmd_read = first;
                }
            } else
            {
                if (((cmd_write - line) + 1) < (CMD_MAXLINE - 1))
                {
                    *cmd_write++ = *cmd_read++;
                }
            }
        }

        *cmd_write = 0;

        /**
         * Armazena a localização atual e fecha o arquivo bat.
         */
        this->location = 0;
        DOS_SeekFile(file_handle, &(this->location), DOS_SEEK_CUR);
        DOS_CloseFile(file_handle);

        return true;
}

/**
 *
 */
bool BatchFile::Goto(char * where)
{
    /**
     * Abra o arquivo bat e procure a string where.
     */
    if (!DOS_OpenFile(filename.c_str(), (DOS_NOT_INHERIT | OPEN_READ), &file_handle))
    {
        LOG(LOG_MISC, LOG_ERROR)("SHELL:Goto Can't open BatchFile %s", filename.c_str());
        delete this;

        return false;
    }

    char cmd_buffer[CMD_MAXLINE];
    char * cmd_write;

    /**
     * Escaneie até que tenhamos uma correspondência ou
     * retorne false.
     */
    Bit8u c;
    Bit16u n;

    again:
        cmd_write = cmd_buffer;

        do
        {
            n = 1;
            DOS_ReadFile(file_handle, &c, &n);

            if (n > 0)
            {
                if (c > 31)
                {
                    if (((cmd_write - cmd_buffer) + 1) < (CMD_MAXLINE - 1))
                    {
                        *cmd_write++ = c;
                    }
                }
            }
        } while (c!='\n' && n);

        *cmd_write++ = 0;
        char *nospace = trim(cmd_buffer);

        if (nospace[0] == ':')
        {
            /**
             * Pulinhos em :
             */
            nospace++;

            /**
             * Tira espaços e = dele.
             */
            while(*nospace && (isspace(*reinterpret_cast<unsigned char*>(nospace)) || (*nospace == '=')))
            {
                nospace++;
            }

            /**
             * O rótulo é até espaço/=/eol.
             */
            char* const beginlabel = nospace;

            while(*nospace && !isspace(*reinterpret_cast<unsigned char*>(nospace)) && (*nospace != '='))
            {
                nospace++;
            }

            *nospace = 0;

            if (strcasecmp(beginlabel, where) == 0)
            {
                /**
                 * Encontrei !
                 * Armazenar localização e continuar.
                 */
                this->location = 0;
                DOS_SeekFile(file_handle, &(this->location), DOS_SEEK_CUR);
                DOS_CloseFile(file_handle);

                return true;
            }
        }

        if (!n)
        {
            DOS_CloseFile(file_handle);
            delete this;

            return false;
        }

        goto again;

        return false;
}

/**
 *
 */
void BatchFile::Shift(void)
{
    cmd->Shift(1);
}
