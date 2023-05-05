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
 * $Id: shell.h,v 1.28 2009-07-03 19:36:57 qbix79 Exp $.
 */

#ifndef DOSBOX_SHELL_H
#define DOSBOX_SHELL_H

    #include <ctype.h>

    #ifndef DOSBOX_DOSBOX_H
    #include "dosbox.h"
    #endif

    #ifndef DOSBOX_PROGRAMS_H
    #include "programs.h"
    #endif

    #include <string>
    #include <list>

    /**
     *
     */
    #define CMD_MAXLINE 4096

    /**
     *
     */
    #define CMD_MAXCMDS 20

    /**
     *
     */
    #define CMD_OLDSIZE 4096

    /**
     *
     */
    extern Bitu call_shellstop;

    /**
     * first_shell é usado para adicionar e excluir itens do
     * shell env por programas "externos". (configuração).
     */
    extern Program * first_shell;

    /**
     *
     */
    class DOS_Shell;

    /**
     *
     */
    class BatchFile
    {
        public:
            /**
             *
             */
            BatchFile(DOS_Shell * host, char const* const resolved_name, char const* const entered_name, char const * const cmd_line);

            /**
             *
             */
            virtual ~BatchFile();

            /**
             *
             */
            virtual bool ReadLine(char * line);

            /**
             *
             */
            bool Goto(char * where);

            /**
             *
             */
            void Shift(void);

            /**
             *
             */
            Bit16u file_handle;

            /**
             *
             */
            Bit32u location;

            /**
             *
             */
            bool echo;

            /**
             *
             */
            DOS_Shell * shell;

            /**
             *
             */
            BatchFile * prev;

            /**
             *
             */
            CommandLine * cmd;

            /**
             *
             */
            std::string filename;
    };

    /**
     *
     */
    class AutoexecEditor;

    /**
     *
     */
    class DOS_Shell : public Program
    {
        private:
            /**
             *
             */
            friend class AutoexecEditor;

            /**
             *
             */
            std::list<std::string> l_history, l_completion;

            /**
             *
             */
            char *completion_start;

            /**
             *
             */
            Bit16u completion_index;

        public:
            /**
             *
             */
            DOS_Shell();

            /**
             *
             */
            void Run(void);

            /**
             * Para gameplay: /C.
             */
            void RunInternal(void);

            /**
             * Uma lista de subfunções.
             */

            /**
             *
             */
            void ParseLine(char * line);

            /**
             *
             */
            Bitu GetRedirection(char *s, char **ifn, char **ofn, bool * append);

            /**
             *
             */
            void InputCommand(char * line);

            /**
             *
             */
            void ShowPrompt();

            /**
             *
             */
            void DoCommand(char * cmd);

            /**
             *
             */
            bool Execute(char * name, char * args);

            /**
             * Verifica se corresponde a uma propriedade de hardware.
             */
            bool CheckConfig(char* cmd_in, char*line);

            /**
             * Algumas funções internas usadas.
             */
            char * Which(char * name);

            /**
             * Algumas gameplay suportadas.
             */

            /**
             *
             */
            void CMD_HELP(char * args);

            /**
             *
             */
            void CMD_CLS(char * args);

            /**
             *
             */
            void CMD_COPY(char * args);

            /**
             *
             */
            void CMD_DIR(char * args);

            /**
             *
             */
            void CMD_DELETE(char * args);

            /**
             *
             */
            void CMD_ECHO(char * args);

            /**
             *
             */
            void CMD_EXIT(char * args);

            /**
             *
             */
            void CMD_MKDIR(char * args);

            /**
             *
             */
            void CMD_CHDIR(char * args);

            /**
             *
             */
            void CMD_RMDIR(char * args);

            /**
             *
             */
            void CMD_SET(char * args);

            /**
             *
             */
            void CMD_IF(char * args);

            /**
             *
             */
            void CMD_GOTO(char * args);

            /**
             *
             */
            void CMD_TYPE(char * args);

            /**
             *
             */
            void CMD_REM(char * args);

            /**
             *
             */
            void CMD_RENAME(char * args);

            /**
             *
             */
            void CMD_CALL(char * args);

            /**
             *
             */
            void SyntaxError(void);

            /**
             *
             */
            void CMD_PAUSE(char * args);

            /**
             *
             */
            void CMD_SUBST(char* args);

            /**
             *
             */
            void CMD_LOADHIGH(char* args);

            /**
             *
             */
            void CMD_CHOICE(char * args);

            /**
             *
             */
            void CMD_ATTRIB(char * args);

            /**
             *
             */
            void CMD_PATH(char * args);

            /**
             *
             */
            void CMD_SHIFT(char * args);

            /**
             *
             */
            void CMD_VER(char * args);

            /**
             * As variáveis do shell.
             */

            /**
             *
             */
            Bit16u input_handle;

            /**
             *
             */
            BatchFile * bf;

            /**
             *
             */
            bool echo;

            /**
             *
             */
            bool exit;

            /**
             *
             */
            bool call;
    };

    /**
     *
     */
    struct SHELL_Cmd
    {
        /**
         * Nome do gameplay.
         */
        const char * name;

        /**
         * Sinalizações sobre o gameplay.
         */
        Bit32u flags;

        /**
         * Manipulador para esta gameplay.
         */
        void (DOS_Shell::*handler)(char * args);

        /**
         * String com ajuda de gameplay.
         */
        const char * help;
    };

    /**
     * Objeto para gerenciar linhas no autoexec.bat.
     * As linhas são removidas do arquivo se o objeto for destruído.
     * O ambiente também é atualizado se a linha for definida como
     * uma variável.
     */
    class AutoexecObject
    {
        private:
            /**
             *
             */
            bool installed;

            /**
             *
             */
            std::string buf;

        public:
            /**
             *
             */
            AutoexecObject():installed(false)
            {
            };

            /**
             *
             */
            void Install(std::string const &in);

            /**
             *
             */
            void InstallBefore(std::string const &in);

            /**
             *
             */
            ~AutoexecObject();

        private:
            /**
             *
             */
            void CreateAutoexec(void);
    };

#endif
