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
 *
 */
void DEBUG_SetupConsole(void);

/**
 *
 */
void DEBUG_DrawScreen(void);

/**
 *
 */
bool DEBUG_Breakpoint(void);

/**
 *
 */
bool DEBUG_IntBreakpoint(Bit8u intNum);

/**
 *
 */
void DEBUG_Enable(bool pressed);

/**
 *
 */
void DEBUG_CheckExecuteBreakpoint(Bit16u seg, Bit32u off);

/**
 *
 */
bool DEBUG_ExitLoop(void);

/**
 *
 */
void DEBUG_RefreshPage(char scroll);

/**
 *
 */
Bitu DEBUG_EnableDebugger(void);

/**
 *
 */
extern Bitu cycle_count;

/**
 *
 */
extern Bitu debugCallback;

/**
 *
 */
#ifdef C_HEAVY_DEBUG
    /**
     *
     */
    bool DEBUG_HeavyIsBreakpoint(void);

    /**
     *
     */
    void DEBUG_HeavyWriteLogInstruction(void);
#endif
