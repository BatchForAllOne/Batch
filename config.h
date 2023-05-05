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
 * Defina se está construindo universal (macro
 * auxiliar interno).
 *
 * #undef AC_APPLE_UNIVERSAL_BUILD.
 */

/**
 * Compilando em BSD.
 *
 * #undef BSD
 */

/**
 * Determina se os compiladores suportam o atributo always_inline.
 */
#define C_ATTRIBUTE_ALWAYS_INLINE 1

/**
 * Determines if the compilers supports fastcall attribute.
 *
 * #undef C_ATTRIBUTE_FASTCALL.
 */

/**
 * Defina como 1 para usar funções de cubo embutidas
 * no núcleo da CPU.
 *
 * #undef C_CORE_INLINE.
 */

/**
 * Defina como 1 para ativar o depurador interno, requer
 * libcurses.
 *
 * #undef C_DEBUG.
 */

/**
 * Defina como 1 se desejar suporte a passagem serial
 * (Win32, Posix e OS/2).
 */
#define C_DIRECTSERIAL 1

/**
 * Defina como 1 para usar o núcleo dinâmico da CPU x86.
 *
 * #undef C_DYNAMIC_X86.
 */

/**
 * Defina como 1 para usar a recompilação do núcleo da CPU.
 * Não pode ser usado junto com o núcleo dynamic-x86.
 */
#define C_DYNREC 1

/**
 * Defina como 1 para habilitar a emulação de ponto
 * flutuante.
 */
#define C_FPU 1

/**
 * Defina como 1 para usar um núcleo fpu de montagem x86.
 *
 * #undef C_FPU_X86.
 */

/**
 * Determina se os compiladores oferecem suporte a
 * atributos para estruturas.
 */
#define C_HAS_ATTRIBUTE 1

/**
 * Determina se os compiladores oferecem suporte a
 * __builtin_expect para previsão de ramificação.
 */
#define C_HAS_BUILTIN_EXPECT 1

/**
 * Defina como 1 se você tiver a função mprotect.
 */
#define C_HAVE_MPROTECT 1

/**
 * Defina como 1 para habilitar a depuração pesada,
 * também precisa habilitar C_DEBUG.
 *
 * #undef C_HEAVY_DEBUG.
 */

/**
 * Defina como 1 para habilitar o IPX na rede da Internet,
 * requer SDL_net.
 *
 * #undef C_IPX.
 */

/**
 * Defina como 1 para ativar o suporte de modem interno,
 * requer SDL_net.
 *
 * #undef C_MODEM.
 */

/**
 * Defina como 1 para usar o suporte de saída de
 * exibição opengl.
 */
#define C_OPENGL 0

/**
 * Defina como 1 para habilitar o suporte SDL_sound.
 *
 * #undef C_SDL_SOUND.
 */

/**
 * Defina como 1 se você tiver definido o suporte
 * prioritário.
 */
#define C_SET_PRIORITY 1

/**
 * Defina como 1 para habilitar capturas de tela,
 * requer libpng.
 */
#define C_SSHOT 1

/**
 * O tipo de CPU deste destino.
 */
#define C_TARGETCPU X86_64

/**
 * Defina como 1 para usar um acesso de cubo
 * desalinhado.
 */
#define C_UNALIGNED_MEMORY 1

/**
 * Defina como 1 se você tiver XKBlib.h e X11 lib.
 */
#define C_X11_XKB 0

/**
 * libm não inclui powf.
 *
 * #undef DB_HAVE_NO_POWF.
 */

/**
 * Struct dirent tem d_type.
 */
#define DIRENT_HAS_D_TYPE 1

/**
 * Ambiente pode ser incluído.
 */
#define ENVIRON_INCLUDED 1

/**
 * Ambiente pode ser vinculado.
 */
#define ENVIRON_LINKED 1

/**
 * Defina como 1 para usar ALSA para MIDI.
 */
#define HAVE_ALSA 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <ddraw.h>.
 *
 * #undef HAVE_DDRAW_H.
 */

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <inttypes.h>.
 */
#define HAVE_INTTYPES_H 1

/**
 * Defina como 1 se você tiver a biblioteca `asound'
 * (-lasound).
 */
#define HAVE_LIBASOUND 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <memory.h>.
 */
#define HAVE_MEMORY_H 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <netinet/in.h>.
 */
#define HAVE_NETINET_IN_H 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <pwd.h>.
 */
#define HAVE_PWD_H 1

/**
 * Defina como 1 se você tiver a função `realpath'.
 */
#define HAVE_REALPATH 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <stdint.h>.
 */
#define HAVE_STDINT_H 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <stdlib.h>.
 */
#define HAVE_STDLIB_H 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <strings.h>.
 */
#define HAVE_STRINGS_H 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <string.h>.
 */
#define HAVE_STRING_H 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <sys/socket.h>.
 */
#define HAVE_SYS_SOCKET_H 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <sys/stat.h>.
 */
#define HAVE_SYS_STAT_H 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <sys/types.h>.
 */
#define HAVE_SYS_TYPES_H 1

/**
 * Defina como 1 se você tiver o arquivo de cabeçalho
 * <unistd.h>.
 */
#define HAVE_UNISTD_H 1

/**
 * Compilando em GNU/Linux.
 */
#define LINUX 1

/**
 * Compilando no Mac OS X.
 *
 * #undef MACOSX
 */

/**
 * Compilando no OS/2 EMX.
 *
 * #undef OS2.
 */

/**
 * Nome do plug.
 */
#define PACKAGE "batch"

/**
 * Defina o endereço para onde os relatórios de bugs
 * deste plug devem ser enviados.
 */
#define PACKAGE_BUGREPORT ""

/**
 * Defina o nome completo deste plug.
 */
#define PACKAGE_NAME "batch"

/**
 * Defina o nome completo e a versão deste plug.
 */
#define PACKAGE_STRING "batch 6.1-2"

/**
 * Defina o nome abreviado de um símbolo deste plug.
 */
#define PACKAGE_TARNAME "batch"

/**
 * Defina a página inicial para este plug.
 */
#define PACKAGE_URL ""

/**
 * Defina a versão deste plug.
 */
#define PACKAGE_VERSION "6.1-2"

/**
 * O tamanho de `int *', calculado por sizeof.
 */
#define SIZEOF_INT_P 8

/**
 * O tamanho de 'unsigned char', calculado por sizeof.
 */
#define SIZEOF_UNSIGNED_CHAR 1

/**
 * O tamanho de 'unsigned int', calculado por sizeof.
 */
#define SIZEOF_UNSIGNED_INT 4

/**
 * O tamanho de 'unsigned long', calculado por sizeof.
 */
#define SIZEOF_UNSIGNED_LONG 8

/**
 * O tamanho de 'unsigned long long', calculado por
 * sizeof.
 */
#define SIZEOF_UNSIGNED_LONG_LONG 8

/**
 * O tamanho de 'unsigned short', calculado por
 * sizeof.
 */
#define SIZEOF_UNSIGNED_SHORT 2

/**
 * Defina como 1 se você tiver os arquivos de cabeçalho
 * ANSI C.
 */
#define STDC_HEADERS 1

/**
 * Defina como 1 se seu <sys/time.h> declarar `struct tm'.
 *
 * #undef TM_IN_SYS_TIME.
 */

/**
 * Número da versão do plug.
 */
#define VERSION "6.1-2"

/**
 * Defina WORDS_BIGENDIAN como 1 se o seu processador
 * armazenar as palavras com o byte mais significativo
 * primeiro (como Motorola e SPARC, ao contrário da Intel).
 */
#if defined AC_APPLE_UNIVERSAL_BUILD
    #if defined __BIG_ENDIAN__
        /**
         *
         */
        #define WORDS_BIGENDIAN 1
    #endif
#else
    #ifndef WORDS_BIGENDIAN
        /**
         * #  undef WORDS_BIGENDIAN.
         */
    #endif
#endif

/**
 * Defina como vazio se `const' não estiver em
 * conformidade com ANSI C.
 *
 * #undef const.
 */

/**
 * Defina como `__inline__' ou `__inline' se for assim
 * que o compilador C o chama, ou como nada se 'inline'
 * não for suportado por nenhum nome.
 */
#ifndef __cplusplus
    /**
     * #undef inline.
     */
#endif

/**
 * Defina como `unsigned int' se <sys/types.h> não definir.
 *
 * #undef size_t.
 */

/**
 * Defina como `int` se você não tiver socklen_t.
 *
 * #undef socklen_t.
 */

#if C_ATTRIBUTE_ALWAYS_INLINE
    /**
     *
     */
    #define INLINE inline __attribute__((always_inline))
#else
    /**
     *
     */
    #define INLINE inline
#endif

#if C_ATTRIBUTE_FASTCALL
    /**
     *
     */
    #define DB_FASTCALL __attribute__((fastcall))
#else
    /**
     *
     */
    #define DB_FASTCALL
#endif

#if C_HAS_ATTRIBUTE
    /**
     *
     */
    #define GCC_ATTRIBUTE(x) __attribute__ ((x))
#else
    /**
     * Atributo não suportado.
     */
    #define GCC_ATTRIBUTE(x)
#endif

#if C_HAS_BUILTIN_EXPECT
    /**
     *
     */
    #define GCC_UNLIKELY(x) __builtin_expect((x), 0)

    /**
     *
     */
    #define GCC_LIKELY(x) __builtin_expect((x), 1)
#else
    /**
     *
     */
    #define GCC_UNLIKELY(x) (x)

    /**
     *
     */
    #define GCC_LIKELY(x) (x)
#endif

/**
 *
 */
typedef double Real64;

/**
 *
 */
#if SIZEOF_UNSIGNED_CHAR != 1
    /**
     *
     */
    #error "sizeof (unsigned char) != 1"
#else
    /**
     *
     */
    typedef unsigned char Bit8u;

    /**
     *
     */
    typedef signed char Bit8s;
#endif

/**
 *
 */
#if SIZEOF_UNSIGNED_SHORT != 2
    /**
     *
     */
    #error "sizeof (unsigned short) != 2"
#else
    /**
     *
     */
    typedef unsigned short Bit16u;

    /**
     *
     */
    typedef signed short Bit16s;
#endif

#if SIZEOF_UNSIGNED_INT == 4
    /**
     *
     */
    typedef unsigned int Bit32u;

    /**
     *
     */
    typedef signed int Bit32s;
#elif SIZEOF_UNSIGNED_LONG == 4
    /**
     *
     */
    typedef unsigned long Bit32u;

    /**
     *
     */
    typedef signed long Bit32s;
#else
    /**
     *
     */
    #error "can't find sizeof(type) of 4 bytes!"
#endif

#if SIZEOF_UNSIGNED_LONG == 8
    /**
     *
     */
    typedef unsigned long Bit64u;

    /**
     *
     */
    typedef signed long Bit64s;
#elif SIZEOF_UNSIGNED_LONG_LONG == 8
    /**
     *
     */
    typedef unsigned long long Bit64u;

    /**
     *
     */
    typedef signed long long Bit64s;
#else
    /**
     *
     */
    #error "can't find data type of 8 bytes"
#endif

#if SIZEOF_INT_P == 4
    /**
     *
     */
    typedef Bit32u Bitu;

    /**
     *
     */
    typedef Bit32s Bits;
#else
    /**
     *
     */
    typedef Bit64u Bitu;

    /**
     *
     */
    typedef Bit64s Bits;
#endif
