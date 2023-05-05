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
 * $Id: video.h,v 1.26 2009-05-27 09:15:41 qbix79 Exp $.
 */


#ifndef DOSBOX_VIDEO_H
#define DOSBOX_VIDEO_H

    /**
     *
     */
    #define REDUCE_JOYSTICK_POLLING

    /**
     *
     */
    typedef enum {
        GFX_CallBackReset,
        GFX_CallBackStop,
        GFX_CallBackRedraw
    } GFX_CallBackFunctions_t;

    /**
     *
     */
    typedef void (*GFX_CallBack_t)(GFX_CallBackFunctions_t function);

    /**
     *
     */
    struct GFX_PalEntry {
        /**
         *
         */
        Bit8u r;

        /**
         *
         */
        Bit8u g;

        /**
         *
         */
        Bit8u b;

        /**
         *
         */
        Bit8u unused;
    };

    /**
     *
     */
    #define GFX_CAN_8 0x0001

    /**
     *
     */
    #define GFX_CAN_15 0x0002

    /**
     *
     */
    #define GFX_CAN_16 0x0004

    /**
     *
     */
    #define GFX_CAN_32 0x0008

    /**
     *
     */
    #define GFX_LOVE_8 0x0010

    /**
     *
     */
    #define GFX_LOVE_15 0x0020

    /**
     *
     */
    #define GFX_LOVE_16 0x0040

    /**
     *
     */
    #define GFX_LOVE_32 0x0080

    /**
     *
     */
    #define GFX_RGBONLY 0x0100

    /**
     *
     */
    #define GFX_SCALING 0x1000

    /**
     *
     */
    #define GFX_HARDWARE 0x2000

    /**
     * Se a interface também pode fazer superfície de
     * acesso aleatório.
     */
    #define GFX_CAN_RANDOM 0x4000

    /**
     *
     */
    void GFX_Events(void);

    /**
     *
     */
    void GFX_SetPalette(Bitu start, Bitu count, GFX_PalEntry * entries);

    /**
     *
     */
    Bitu GFX_GetBestMode(Bitu flags);

    /**
     *
     */
    Bitu GFX_GetRGB(Bit8u red, Bit8u green, Bit8u blue);

    /**
     *
     */
    Bitu GFX_SetSize(Bitu width, Bitu height, Bitu flags, double scalex, double scaley, GFX_CallBack_t cb);

    /**
     *
     */
    void GFX_ResetScreen(void);

    /**
     *
     */
    void GFX_Start(void);

    /**
     *
     */
    void GFX_Stop(void);

    /**
     *
     */
    void GFX_SwitchFullScreen(void);

    /**
     *
     */
    bool GFX_StartUpdate(Bit8u * & pixels, Bitu & pitch);

    /**
     *
     */
    void GFX_EndUpdate(const Bit16u *changedLines);

    /**
     *
     */
    void GFX_GetSize(int &width, int &height, bool &fullscreen);

    /**
     *
     */
    void GFX_LosingFocus(void);

    /**
     *
     */
    #if defined (WIN32)
        /**
         *
         */
        bool GFX_SDLUsingWinDIB(void);
    #endif

    /**
     *
     */
    #if defined (REDUCE_JOYSTICK_POLLING)
        /**
         *
         */
        void MAPPER_UpdateJoysticks(void);
    #endif

    /**
     * Relacionado ao objeto de controle.
     */
    void GFX_CaptureMouse(void);

    /**
     * Verdadeiro se o objeto de controle estiver confinado à janela.
     */
    extern bool mouselocked;

#endif
