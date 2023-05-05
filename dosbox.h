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
 * $Id: dosbox.h,v 1.32 2009-05-27 09:15:41 qbix79 Exp $.
 */

#ifndef DOSBOX_DOSBOX_H
#define DOSBOX_DOSBOX_H

    #include "config.h"


    /**
     *
     */
    GCC_ATTRIBUTE(noreturn) void E_Exit(const char * message,...) GCC_ATTRIBUTE( __format__(__printf__, 1, 2));

    /**
     * Adicionar mensagens ao langaugefile interno.
     */
    void MSG_Add(const char*, const char*);

    /**
     * Obter mensagens do langaugafile interno.
     */
    const char* MSG_Get(char const *);

    /**
     *
     */
    class Section;

    /**
     *
     */
    typedef Bitu (LoopHandler)(void);

    /**
     *
     */
    void DOSBOX_RunMachine();

    /**
     *
     */
    void DOSBOX_SetLoop(LoopHandler * handler);

    /**
     *
     */
    void DOSBOX_SetNormalLoop();

    /**
     *
     */
    void DOSBOX_Init(void);

    /**
     *
     */
    class Config;

    /**
     *
     */
    extern Config * control;

    /**
     *
     */
    enum MachineType
    {
        MCH_HERC,
        MCH_CGA,
        MCH_TANDY,
        MCH_PCJR,
        MCH_EGA,
        MCH_VGA
    };

    /**
     *
     */
    enum SVGACards
    {
        SVGA_None,
        SVGA_S3Trio,
        SVGA_TsengET4K,
        SVGA_TsengET3K,
        SVGA_ParadisePVGA1A
    };

    /**
     *
     */
    extern SVGACards svgaCard;

    /**
     *
     */
    extern MachineType machine;

    /**
     *
     */
    extern bool SDLNetInited;

    /**
     *
     */
    #define IS_TANDY_ARCH ((machine == MCH_TANDY) || (machine == MCH_PCJR))

    /**
     *
     */
    #define IS_EGAVGA_ARCH ((machine == MCH_EGA) || (machine == MCH_VGA))

    /**
     *
     */
    #define IS_VGA_ARCH (machine == MCH_VGA)

    /**
     *
     */
    #define TANDY_ARCH_CASE MCH_TANDY: case MCH_PCJR

    /**
     *
     */
    #define EGAVGA_ARCH_CASE MCH_EGA: case MCH_VGA

    /**
     *
     */
    #define VGA_ARCH_CASE MCH_VGA

    /**
     * O sistema de cadastro.
     */
    #ifndef DOSBOX_LOGGING_H
        /**
         *
         */
        #include "logging.h"
    #endif

#endif
