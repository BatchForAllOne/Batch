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
 * $Id: callback.h,v 1.26 2009-08-23 17:24:54 c2woody Exp $.
 */

#ifndef DOSBOX_CALLBACK_H
#define DOSBOX_CALLBACK_H

    #ifndef DOSBOX_MEM_H
    #include "mem.h"
    #endif

    /**
     *
     */
    typedef Bitu(*CallBack_Handler)(void);

    /**
     *
     */
    extern CallBack_Handler CallBack_Handlers[];

    /**
     *
     */
    enum {
        CB_RETN,
        CB_RETF,
        CB_RETF8,
        CB_IRET,
        CB_IRETD,
        CB_IRET_STI,
        CB_IRET_EOI_PIC1,
        CB_IRQ0,
        CB_IRQ1,
        CB_IRQ9,
        CB_IRQ12,
        CB_IRQ12_RET,
        CB_IRQ6_PCJR,
        CB_MOUSE,
        CB_INT29,
        CB_INT16,
        CB_HOOKABLE,
        CB_TDE_IRET,
        CB_IPXESR,
        CB_IPXESR_RET,
        CB_INT21
    };

    /**
     *
     */
    #define CB_MAX 128

    /**
     *
     */
    #define CB_SIZE 32

    /**
     *
     */
    #define CB_SEG 0xF000

    /**
     *
     */
    #define CB_SOFFSET 0x1000

    /**
     *
     */
    enum {
        CBRET_NONE = 0,
        CBRET_STOP = 1
    };

    /**
     *
     */
    extern Bit8u lastint;

    /**
     *
     */
    static INLINE RealPt CALLBACK_RealPointer(Bitu callback)
    {
        return RealMake(CB_SEG, (Bit16u)(CB_SOFFSET + callback * CB_SIZE));
    }

    /**
     *
     */
    static INLINE PhysPt CALLBACK_PhysPointer(Bitu callback)
    {
        return PhysMake(CB_SEG, (Bit16u)(CB_SOFFSET + callback * CB_SIZE));
    }

    /**
     *
     */
    static INLINE PhysPt CALLBACK_GetBase(void)
    {
        return (CB_SEG << 4) + CB_SOFFSET;
    }

    /**
     *
     */
    Bitu CALLBACK_Allocate();

    /**
     *
     */
    void CALLBACK_Idle(void);

    /**
     *
     */
    void CALLBACK_RunRealInt(Bit8u intnum);

    /**
     *
     */
    void CALLBACK_RunRealFar(Bit16u seg, Bit16u off);

    /**
     *
     */
    bool CALLBACK_Setup(Bitu callback, CallBack_Handler handler, Bitu type, const char* descr);

    /**
     *
     */
    Bitu CALLBACK_Setup(Bitu callback, CallBack_Handler handler, Bitu type, PhysPt addr, const char* descr);

    /**
     *
     */
    const char* CALLBACK_GetDescription(Bitu callback);

    /**
     *
     */
    bool CALLBACK_Free(Bitu callback);

    /**
     *
     */
    void CALLBACK_SCF(bool val);

    /**
     *
     */
    void CALLBACK_SZF(bool val);

    /**
     *
     */
    void CALLBACK_SIF(bool val);

    /**
     *
     */
    extern Bitu call_priv_io;

    /**
     *
     */
    class CALLBACK_HandlerObject
    {
        private:
            /**
             *
             */
            bool installed;

            /**
             *
             */
            Bitu m_callback;

            /**
             *
             */
            enum {
                NONE,
                SETUP,
                SETUPAT
            } m_type;

            /**
             *
             */
            struct {
                /**
                 *
                 */
                RealPt old_vector;

                /**
                 *
                 */
                Bit8u interrupt;

                /**
                 *
                 */
                bool installed;
            } vectorhandler;

        public:
            /**
             *
             */
            CALLBACK_HandlerObject(): installed(false), m_type(NONE)
            {
                vectorhandler.installed = false;
            }

            /**
             *
             */
            ~CALLBACK_HandlerObject();

            /**
             * Instale e aloque um retorno de chamada.
             */
            void Install(CallBack_Handler handler, Bitu type, const char* description);

            /**
             *
             */
            void Install(CallBack_Handler handler, Bitu type, PhysPt addr, const char* description);

            /**
             * Atribua apenas um número de retorno de chamada.
             */
            void Allocate(CallBack_Handler handler, const char* description = 0);

            /**
             *
             */
            Bit16u Get_callback()
            {
                return (Bit16u)m_callback;
            }

            /**
             *
             */
            RealPt Get_RealPointer()
            {
                return CALLBACK_RealPointer(m_callback);
            }

            /**
             *
             */
            void Set_RealVec(Bit8u vec);
    };

#endif
