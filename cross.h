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
 * $Id: cross.h,v 1.21 2009-03-14 18:02:34 qbix79 Exp $.
 */

#ifndef DOSBOX_CROSS_H
#define DOSBOX_CROSS_H

    #ifndef DOSBOX_DOSBOX_H
        /**
         *
         */
        #include "dosbox.h"
    #endif

    #include <stdio.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <string>

    #if defined (_MSC_VER)
        /**
         * MS Visual C++.
         */
        #include <direct.h>
        #include <io.h>

        #define LONGTYPE(a) a##i64
        #define snprintf _snprintf
        #define vsnprintf _vsnprintf
    #else
        /**
         * LINUX / GCC.
         */
        #include <dirent.h>
        #include <unistd.h>

        #define LONGTYPE(a) a##LL
    #endif

    /**
     * Tamanho máximo do nome do arquivo.
     */
    #define CROSS_LEN 512

    #if defined (WIN32) || defined (OS2)
        /**
         * Win 32 & OS/2.
         */
        #define CROSS_FILENAME(blah)
        #define CROSS_FILESPLIT '\\'
        #define F_OK 0
    #else
        #define CROSS_FILENAME(blah) strreplace(blah,'\\','/')
        #define CROSS_FILESPLIT '/'
    #endif

    #define CROSS_NONE 0
    #define CROSS_FILE 1
    #define CROSS_DIR 2

    #if defined (WIN32)
        /**
         *
         */
        #define ftruncate(blah, blah2) chsize(blah, blah2)
    #endif

    /**
     * Solaris talvez outros.
     */
    #if defined (DB_HAVE_NO_POWF)
        #include <math.h>

        /**
         *
         */
        static inline float powf(float x, float y)
        {
            return (float) pow(x, y);
        }
    #endif

    /**
     *
     */
    class Cross
    {
        public:
            /**
             *
             */
            static void GetPlatformConfigDir(std::string& in);

            /**
             *
             */
            static void GetPlatformConfigName(std::string& in);

            /**
             *
             */
            static void CreatePlatformConfigDir(std::string& in);

            /**
             *
             */
            static void ResolveHomedir(std::string & temp_line);

            /**
             *
             */
            static void CreateDir(std::string const& temp);
    };

    /**
     *
     */
    #if defined (WIN32)
        /**
         * Exclua itens raramente usados de:
         */
        #define WIN32_LEAN_AND_MEAN

        /**
         *
         */
        #include <windows.h>

        /**
         *
         */
        typedef struct dir_struct
        {
            /**
             *
             */
            HANDLE handle;

            /**
             *
             */
            char base_path[MAX_PATH + 4];

            /**
             *
             */
            WIN32_FIND_DATA search_data;
        } dir_information;
    #else
        /**
         * #include <sys/types.h> - Incluído acima.
         */
        #include <dirent.h>

        /**
         *
         */
        typedef struct dir_struct
        {
            /**
             *
             */
            DIR* dir;

            /**
             *
             */
            char base_path[CROSS_LEN];
        } dir_information;
    #endif

    /**
     *
     */
    dir_information* open_directory(const char* dirname);

    /**
     *
     */
    bool read_directory_first(dir_information* dirp, char* entry_name, bool& is_directory);

    /**
     *
     */
    bool read_directory_next(dir_information* dirp, char* entry_name, bool& is_directory);

    /**
     *
     */
    void close_directory(dir_information* dirp);

    /**
     *
     */
    FILE *fopen_wrap(const char *path, const char *mode);

#endif
