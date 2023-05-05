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
 * $Id: support.cpp,v 1.37 2009-05-27 09:15:42 qbix79 Exp $.
 */


#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <cctype>
#include <string>

#include "dosbox.h"
#include "debug.h"
#include "support.h"
#include "video.h"


/**
 *
 */
void upcase(std::string &str)
{
    int (*tf)(int) = std::toupper;

    std::transform(str.begin(), str.end(), str.begin(), tf);
}

/**
 *
 */
void lowcase(std::string &str)
{
    int (*tf)(int) = std::tolower;

    std::transform(str.begin(), str.end(), str.begin(), tf);
}

/**
 * Fez alteração de alguma fonte do FreeDOS para este.
 * Substitui todas as instâncias do caractere o pelo
 * caractere c.
 */
void strreplace(char * str, char o, char n)
{
    while (*str)
    {
        if (*str == o)
        {
            *str = n;
        }

        str++;
    }
}

/**
 *
 */
char *ltrim(char *str)
{
    while (*str && isspace(*reinterpret_cast<unsigned char*>(str)))
    {
        str++;
    }

    return str;
}

/**
 *
 */
char *rtrim(char *str)
{
    char *p;
    p = strchr(str, '\0');

    while (--p >= str && isspace(*reinterpret_cast<unsigned char*>(p)))
    {
    };

    p[1] = '\0';

    return str;
}

/**
 *
 */
char *trim(char *str)
{
    return ltrim(rtrim(str));
}

/**
 *
 */
char * upcase(char * str)
{
    for (char* idx = str; *idx; idx++)
    {
        *idx = toupper(*reinterpret_cast<unsigned char*>(idx));
    }

    return str;
}

/**
 *
 */
char * lowcase(char * str)
{
    for(char* idx = str; *idx; idx++)
    {
        *idx = tolower(*reinterpret_cast<unsigned char*>(idx));
    }

    return str;
}

/**
 *
 */
bool ScanCMDBool(char * cmd, char const * const check)
{
    char * scan = cmd;
    size_t c_len = strlen(check);

    while ((scan = strchr(scan, '/')))
    {
        /**
         * Encontrei um `/` agora, consigo ver atrás dele.
         */
        scan++;

        if (strncasecmp(scan, check, c_len) == 0 && (scan[c_len] == ' ' || scan[c_len] == '\t' || scan[c_len] == '/' || scan[c_len] == 0))
        {
            /**
             * Encontrou uma matemática, agora remova-a da sequência.
             */
            memmove(scan - 1, scan + c_len, strlen(scan + c_len) + 1);
            trim(scan - 1);

            return true;
        }
    }

    return false;
}

/**
 * Isso verifica a linha de comando em busca de uma opção
 * restante e relata que, caso contrário, retorna 0.
 */
char * ScanCMDRemain(char * cmd)
{
    char * scan;
    char *found;

    if ((scan = found = strchr(cmd, '/')))
    {
        while (*scan && !isspace(*reinterpret_cast<unsigned char*>(scan)))
        {
            scan++;
        }

        *scan = 0;

        return found;
    } else
    {
        return 0;
    }
}

/**
 *
 */
char * StripWord(char *&line)
{
    char * scan = line;
    scan = ltrim(scan);

    if (*scan == '"')
    {
        char * end_quote = strchr(scan + 1, '"');

        if (end_quote)
        {
            *end_quote = 0;
            line = ltrim(++end_quote);

            return (scan+1);
        }
    }

    char * begin = scan;
    for (char c = *scan; (c = *scan); scan++)
    {
        if (isspace(*reinterpret_cast<unsigned char*>(&c)))
        {
            *scan++ = 0;
            break;
        }
    }

    line = scan;

    return begin;
}

/**
 *
 */
Bits ConvDecWord(char * word)
{
    bool negative = false;
    Bitu ret = 0;

    if (*word == '-')
    {
        negative = true;
        word++;
    }

    while (char c = *word)
    {
        ret* = 10;
        ret += c - '0';
        word++;
    }

    if (negative)
    {
        return 0 - ret;
    } else
    {
        return ret;
    }
}

/**
 *
 */
Bits ConvHexWord(char * word)
{
    Bitu ret = 0;
    while (char c = toupper(*reinterpret_cast<unsigned char*>(word)))
    {
        ret* = 16;

        if (c >= '0' && c <= '9')
        {
            ret += c - '0';
        } else if (c >= 'A' && c <= 'F')
        {
            ret += 10 + (c - 'A');
        }

        word++;
    }

    return ret;
}

/**
 *
 */
double ConvDblWord(char * word)
{
    return 0.0f;
}

/**
 * Escopo maior, caso contrário, nem sempre é lançado
 * corretamente (Eu faço o `L` - Gcc-xxx).
 */
static char buf[1024];

/**
 *
 */
void E_Exit(const char * format, ...)
{
    #if C_DEBUG && C_HEAVY_DEBUG
        DEBUG_HeavyWriteLogInstruction();
    #endif

    va_list msg;
    va_start(msg, format);
    vsprintf(buf, format, msg);
    va_end(msg);
    strcat(buf, "\n");

    throw(buf);
}
