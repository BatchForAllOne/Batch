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
 * $Id: control.h,v 1.2 2009-05-27 09:15:40 qbix79 Exp $.
 */

#ifndef DOSBOX_CONTROL_H
#define DOSBOX_CONTROL_H

    /**
     *
     */
    #ifdef _MSC_VER
        /**
         *
         */
        #pragma warning ( disable : 4786 )

        /**
         *
         */
        #pragma warning ( disable : 4290 )
    #endif

    /**
     *
     */
    #ifndef DOSBOX_PROGRAMS_H
    #include "programs.h"
    #endif

    /**
     *
     */
    #ifndef DOSBOX_SETUP_H
    #include "setup.h"
    #endif

    /**
     *
     */
    #ifndef CH_LIST
    #define CH_LIST
        /**
         *
         */
        #include <list>
    #endif

    /**
     *
     */
    #ifndef CH_VECTOR
    #define CH_VECTOR
        /**
         *
         */
        #include <vector>
    #endif

    /**
     *
     */
    #ifndef CH_STRING
    #define CH_STRING
        /**
         *
         */
        #include <string>
    #endif


    /**
     *
     */
    class Config
    {
        public:
            /**
             *
             */
            CommandLine * cmdline;

        private:
            /**
             *
             */
            std::list<Section*> sectionlist;

            /**
             *
             */
            typedef std::list<Section*>::iterator it;

            /**
             *
             */
            typedef std::list<Section*>::reverse_iterator reverse_it;

            /**
             *
             */
            typedef std::list<Section*>::const_iterator const_it;

            /**
             *
             */
            typedef std::list<Section*>::const_reverse_iterator const_reverse_it;

            /**
             *
             */
            void (* _start_function)(void);

            /**
             * Modo Gamer.
             */
            bool secure_mode;

        public:
            /**
             *
             */
            Config(CommandLine * cmd): cmdline(cmd), secure_mode(false)
            {
            }

            /**
             *
             */
            ~Config();

            /**
             *
             */
            Section_line * AddSection_line(char const * const _name, void (*_initfunction)(Section*));

            /**
             *
             */
            Section_prop * AddSection_prop(char const * const _name, void (*_initfunction)(Section*), bool canchange = false);

            /**
             *
             */
            Section* GetSection(int index);

            /**
             *
             */
            Section* GetSection(std::string const&_sectionname) const;

            /**
             *
             */
            Section* GetSectionFromProperty(char const * const prop) const;

            /**
             *
             */
            void SetStartUp(void (*_function)(void));

            /**
             *
             */
            void Init();

            /**
             *
             */
            void ShutDown();

            /**
             *
             */
            void StartUp();

            /**
             *
             */
            bool PrintConfig(char const * const configfilename) const;

            /**
             *
             */
            bool ParseConfigFile(char const * const configfilename);

            /**
             *
             */
            void ParseEnv(char ** envp);

            /**
             *
             */
            bool SecureMode() const
            {
                return secure_mode;
            }

            /**
             * Não pode ser desfeito.
             */
            void SwitchToSecureMode()
            {
                secure_mode = true;
            }
    };

#endif
