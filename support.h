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
 * $Id: support.h,v 1.18 2009-05-27 09:15:41 qbix79 Exp $.
 */

#ifndef DOSBOX_SUPPORT_H
#define DOSBOX_SUPPORT_H

    #include <string.h>
    #include <string>
    #include <ctype.h>

    /**
     *
     */
    #ifndef DOSBOX_DOSBOX_H
        /**
         *
         */
        #include "dosbox.h"
    #endif

    /**
     * MS Visual C++.
     */
    #if defined (_MSC_VER)
        /**
         *
         */
        #define strcasecmp(a, b) stricmp(a, b)

        /**
         *
         */
        #define strncasecmp(a, b, n) _strnicmp(a, b, n)
    #endif

    /**
     *
     */
    #define safe_strncpy(a, b, n) \
        do \
        { \
            strncpy((a), (b), (n) - 1); \
            (a)[(n) - 1] = 0; \
        } while (0)

    /**
     *
     */
    #ifdef HAVE_STRINGS_H
        /**
         *
         */
        #include <strings.h>
    #endif

    /**
     *
     */
    void strreplace(char * str, char o, char n);

    /**
     *
     */
    char *ltrim(char *str);

    /**
     *
     */
    char *rtrim(char *str);

    /**
     *
     */
    char *trim(char * str);

    /**
     *
     */
    char * upcase(char * str);

    /**
     *
     */
    char * lowcase(char * str);

    /**
     *
     */
    bool ScanCMDBool(char * cmd, char const * const check);

    /**
     *
     */
    char * ScanCMDRemain(char * cmd);

    /**
     *
     */
    char * StripWord(char *&cmd);

    /**
     *
     */
    bool IsDecWord(char * word);

    /**
     *
     */
    bool IsHexWord(char * word);

    /**
     *
     */
    Bits ConvDecWord(char * word);

    /**
     *
     */
    Bits ConvHexWord(char * word);

    /**
     *
     */
    void upcase(std::string &str);

    /**
     *
     */
    void lowcase(std::string &str);

#endif
