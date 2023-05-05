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
 * $Id: dos_inc.h,v 1.83 2009-10-28 21:45:12 qbix79 Exp $.
 */

#ifndef DOSBOX_DOS_INC_H
#define DOSBOX_DOS_INC_H

    #ifndef DOSBOX_DOS_SYSTEM_H
        /**
         *
         */
        #include "dos_system.h"
    #endif

    #ifndef DOSBOX_MEM_H
        /**
         *
         */
        #include "mem.h"
    #endif

    /**
     * Para compensação de.
     */
    #include <stddef.h>

    /**
     *
     */
    #ifdef _MSC_VER
        /**
         *
         */
        #pragma pack(1)
    #endif

    /**
     *
     */
    struct CommandTail
    {
        /**
         * Número de bytes retornados.
         */
        Bit8u count;

        /**
         * O próprio buffer.
         */
        char buffer[127];
    } GCC_ATTRIBUTE(packed);

    /**
     *
     */
    #ifdef _MSC_VER
        /**
         *
         */
        #pragma pack()
    #endif

    /**
     *
     */
    struct DOS_Date
    {
        /**
         *
         */
        Bit16u year;

        /**
         *
         */
        Bit8u month;

        /**
         *
         */
        Bit8u day;
    };

    /**
     *
     */
    struct DOS_Version
    {
        /**
         *
         */
        Bit8u major, minor, revision;
    };

    /**
     *
     */
    #ifdef _MSC_VER
        /**
         *
         */
        #pragma pack(1)
    #endif

    /**
     *
     */
    union bootSector
    {
        /**
         *
         */
        struct entries
        {
            /**
             *
             */
            Bit8u jump[3];

            /**
             *
             */
            Bit8u oem_name[8];

            /**
             *
             */
            Bit16u bytesect;

            /**
             *
             */
            Bit8u sectclust;

            /**
             *
             */
            Bit16u reserve_sect;

            /**
             *
             */
            Bit8u misc[496];
        } bootdata;

        /**
         *
         */
        Bit8u rawdata[512];
    } GCC_ATTRIBUTE(packed);

    /**
     *
     */
    #ifdef _MSC_VER
        /**
         *
         */
        #pragma pack()
    #endif

    /**
     *
     */
    enum {
        MCB_FREE = 0x0000,
        MCB_DOS = 0x0008
    };

    /**
     *
     */
    enum {
        RETURN_EXIT = 0,
        RETURN_CTRLC = 1,
        RETURN_ABORT = 2,
        RETURN_TSR = 3
    };

    /**
     *
     */
    #define DOS_FILES 127

    /**
     *
     */
    #define DOS_DRIVES 26

    /**
     *
     */
    #define DOS_DEVICES 10

    /**
     * A área intercambiável dos é 0x320 bytes além da tabela
     * sysvars sequência de driver de dispositivo está
     * dentro de sysvars.
     */

    /**
     * sysvars (lista de listas).
     */
    #define DOS_INFOBLOCK_SEG 0x80

    /**
     *
     */
    #define DOS_CONDRV_SEG 0xa0

    /**
     *
     */
    #define DOS_CONSTRING_SEG 0xa8

    /**
     * área trocável dos.
     */
    #define DOS_SDA_SEG 0xb2

    /**
     *
     */
    #define DOS_SDA_OFS 0

    /**
     *
     */
    #define DOS_CDS_SEG 0x108

    /**
     *
     */
    #define DOS_FIRST_SHELL 0x118

    /**
     * Primeiro segmento que o DOS pode usar.
     */
    #define DOS_MEM_START 0x16f

    /**
     *
     */
    #define DOS_PRIVATE_SEGMENT 0xc800

    /**
     *
     */
    #define DOS_PRIVATE_SEGMENT_END 0xd000

    /**
     * Tabelas DOS internas.
     */

    /**
     *
     */
    extern DOS_File * Files[DOS_FILES];

    /**
     *
     */
    extern DOS_Drive * Drives[DOS_DRIVES];

    /**
     *
     */
    extern DOS_Device * Devices[DOS_DEVICES];

    /**
     *
     */
    extern Bit8u dos_copybuf[0x10000];

    /**
     *
     */
    void DOS_SetError(Bit16u code);

    /**
     * Rotinas de Manuseio de Arquivos.
     */

    /**
     *
     */
    enum {
        STDIN = 0,
        STDOUT = 1,
        STDERR = 2,
        STDAUX = 3,
        STDPRN = 4
    };

    /**
     *
     */
    enum {
        HAND_NONE = 0,
        HAND_FILE,
        HAND_DEVICE
    };

    /**
     * Rotinas para classe de arquivo.
     */

    /**
     *
     */
    void DOS_SetupFiles(void);

    /**
     *
     */
    bool DOS_ReadFile(Bit16u handle, Bit8u * data, Bit16u * amount);

    /**
     *
     */
    bool DOS_WriteFile(Bit16u handle, Bit8u * data, Bit16u * amount);

    /**
     *
     */
    bool DOS_SeekFile(Bit16u handle, Bit32u * pos, Bit32u type);

    /**
     *
     */
    bool DOS_CloseFile(Bit16u handle);

    /**
     *
     */
    bool DOS_FlushFile(Bit16u handle);

    /**
     *
     */
    bool DOS_DuplicateEntry(Bit16u entry, Bit16u * newentry);

    /**
     *
     */
    bool DOS_ForceDuplicateEntry(Bit16u entry, Bit16u newentry);

    /**
     *
     */
    bool DOS_GetFileDate(Bit16u entry, Bit16u* otime, Bit16u* odate);

    /**
     * Rotinas para a classe de Driver.
     */

    /**
     *
     */
    bool DOS_OpenFile(char const * name, Bit8u flags, Bit16u * entry);

    /**
     *
     */
    bool DOS_OpenFileExtended(char const * name, Bit16u flags, Bit16u createAttr, Bit16u action, Bit16u *entry, Bit16u* status);

    /**
     *
     */
    bool DOS_CreateFile(char const * name, Bit16u attribute, Bit16u * entry);

    /**
     *
     */
    bool DOS_UnlinkFile(char const * const name);

    /**
     *
     */
    bool DOS_FindFirst(char *search, Bit16u attr, bool fcb_findfirst = false);

    /**
     *
     */
    bool DOS_FindNext(void);

    /**
     *
     */
    bool DOS_Canonicalize(char const * const name, char * const big);

    /**
     *
     */
    bool DOS_CreateTempFile(char * const name, Bit16u * entry);

    /**
     *
     */
    bool DOS_FileExists(char const * const name);

    /**
     * Funções Auxiliares.
     */
    bool DOS_MakeName(char const * const name, char * const fullname, Bit8u * drive);

    /**
     * Rotinas de entrega de unidades.
     */
    Bit8u DOS_GetDefaultDrive(void);

    /**
     *
     */
    void DOS_SetDefaultDrive(Bit8u drive);

    /**
     *
     */
    bool DOS_SetDrive(Bit8u drive);

    /**
     *
     */
    bool DOS_GetCurrentDir(Bit8u drive, char * const buffer);

    /**
     *
     */
    bool DOS_ChangeDir(char const * const dir);

    /**
     *
     */
    bool DOS_MakeDir(char const * const dir);

    /**
     *
     */
    bool DOS_RemoveDir(char const * const dir);

    /**
     *
     */
    bool DOS_Rename(char const * const oldname, char const * const newname);

    /**
     *
     */
    bool DOS_GetFreeDiskSpace(Bit8u drive, Bit16u * bytes, Bit8u * sectors, Bit16u * clusters, Bit16u * free);

    /**
     *
     */
    bool DOS_GetFileAttr(char const * const name, Bit16u * attr);

    /**
     *
     */
    bool DOS_SetFileAttr(char const * const name, Bit16u attr);

    /**
     * Material IOCTL.
     */

    /**
     *
     */
    bool DOS_IOCTL(void);

    /**
     *
     */
    bool DOS_GetSTDINStatus();

    /**
     *
     */
    Bit8u DOS_FindDevice(char const * name);

    /**
     *
     */
    void DOS_SetupDevices(void);

    /**
     * Processar e criar novos processos.
     */

    /**
     *
     */
    bool DOS_NewPSP(Bit16u pspseg, Bit16u size);

    /**
     *
     */
    bool DOS_ChildPSP(Bit16u pspseg, Bit16u size);

    /**
     *
     */
    bool DOS_Execute(char * name, PhysPt block, Bit8u flags);

    /**
     *
     */
    void DOS_Terminate(Bit16u pspseg, bool tsr, Bit8u exitcode);

    /**
     * Rotinas de manipulação de cubos.
     */

    /**
     *
     */
    void DOS_SetupMemory(void);

    /**
     *
     */
    bool DOS_AllocateMemory(Bit16u * segment, Bit16u * blocks);

    /**
     *
     */
    bool DOS_ResizeMemory(Bit16u segment, Bit16u * blocks);

    /**
     *
     */
    bool DOS_FreeMemory(Bit16u segment);

    /**
     *
     */
    void DOS_FreeProcessMemory(Bit16u pspseg);

    /**
     *
     */
    Bit16u DOS_GetMemory(Bit16u pages);

    /**
     *
     */
    bool DOS_SetMemAllocStrategy(Bit16u strat);

    /**
     *
     */
    Bit16u DOS_GetMemAllocStrategy(void);

    /**
     *
     */
    void DOS_BuildUMBChain(bool umb_active, bool ems_active);

    /**
     *
     */
    bool DOS_LinkUMBsToMemChain(Bit16u linkstate);

    /**
     * Coisas do FCB.
     */

    /**
     *
     */
    bool DOS_FCBOpen(Bit16u seg, Bit16u offset);

    /**
     *
     */
    bool DOS_FCBCreate(Bit16u seg, Bit16u offset);

    /**
     *
     */
    bool DOS_FCBClose(Bit16u seg, Bit16u offset);

    /**
     *
     */
    bool DOS_FCBFindFirst(Bit16u seg, Bit16u offset);

    /**
     *
     */
    bool DOS_FCBFindNext(Bit16u seg, Bit16u offset);

    /**
     *
     */
    Bit8u DOS_FCBRead(Bit16u seg, Bit16u offset, Bit16u numBlocks);

    /**
     *
     */
    Bit8u DOS_FCBWrite(Bit16u seg, Bit16u offset, Bit16u numBlocks);

    /**
     *
     */
    Bit8u DOS_FCBRandomRead(Bit16u seg, Bit16u offset, Bit16u numRec, bool restore);

    /**
     *
     */
    Bit8u DOS_FCBRandomWrite(Bit16u seg, Bit16u offset, Bit16u numRec, bool restore);

    /**
     *
     */
    bool DOS_FCBGetFileSize(Bit16u seg, Bit16u offset);

    /**
     *
     */
    bool DOS_FCBDeleteFile(Bit16u seg, Bit16u offset);

    /**
     *
     */
    bool DOS_FCBRenameFile(Bit16u seg, Bit16u offset);

    /**
     *
     */
    void DOS_FCBSetRandomRecord(Bit16u seg, Bit16u offset);

    /**
     *
     */
    Bit8u FCB_Parsename(Bit16u seg, Bit16u offset, Bit8u parser, char *string, Bit8u *change);

    /**
     *
     */
    bool DOS_GetAllocationInfo(Bit8u drive, Bit16u * _bytes_sector, Bit8u * _sectors_cluster, Bit16u * _total_clusters);

    /**
     * Interrupções extras do DOS.
     */
    void DOS_SetupMisc(void);

    /**
     * As tabelas DOS.
     */
    void DOS_SetupTables(void);

    /**
     * Programas internos de configuração do DOS.
     */
    void DOS_SetupPrograms(void);

    /**
     * Inicializar Layout do Teclado.
     */
    void DOS_KeyboardLayout_Init(Section* sec);

    /**
     *
     */
    bool DOS_LayoutKey(Bitu key, Bit8u flags1, Bit8u flags2, Bit8u flags3);

    /**
     *
     */
    enum {
        KEYB_NOERROR = 0,
        KEYB_FILENOTFOUND,
        KEYB_INVALIDFILE,
        KEYB_LAYOUTNOTFOUND,
        KEYB_INVALIDCPFILE
    };

    /**
     *
     */
    static INLINE Bit16u long2para(Bit32u size)
    {
        if (size > 0xFFFF0)
        {
            return 0xffff;
        }

        if (size & 0xf)
        {
            return (Bit16u)((size >> 4) + 1);
        } else
        {
            return (Bit16u)(size >> 4);
        }
    }

    /**
     *
     */
    static INLINE Bit16u DOS_PackTime(Bit16u hour, Bit16u min, Bit16u sec)
    {
        return (hour & 0x1f) << 11 | (min & 0x3f) << 5 | ((sec / 2) & 0x1f);
    }

    /**
     *
     */
    static INLINE Bit16u DOS_PackDate(Bit16u year, Bit16u mon, Bit16u day)
    {
        return ((year - 1980) & 0x7f) << 9 | (mon & 0x3f) << 5 | (day & 0x1f);
    }

    /**
     * Lista de códigos de erro do DOS.
     */

    /**
     *
     */
    #define DOSERR_NONE 0

    /**
     *
     */
    #define DOSERR_FUNCTION_NUMBER_INVALID 1

    /**
     *
     */
    #define DOSERR_FILE_NOT_FOUND 2

    /**
     *
     */
    #define DOSERR_PATH_NOT_FOUND 3

    /**
     *
     */
    #define DOSERR_TOO_MANY_OPEN_FILES 4

    /**
     *
     */
    #define DOSERR_ACCESS_DENIED 5

    /**
     *
     */
    #define DOSERR_INVALID_HANDLE 6

    /**
     *
     */
    #define DOSERR_MCB_DESTROYED 7

    /**
     *
     */
    #define DOSERR_INSUFFICIENT_MEMORY 8

    /**
     *
     */
    #define DOSERR_MB_ADDRESS_INVALID 9

    /**
     *
     */
    #define DOSERR_ENVIRONMENT_INVALID 10

    /**
     *
     */
    #define DOSERR_FORMAT_INVALID 11

    /**
     *
     */
    #define DOSERR_ACCESS_CODE_INVALID 12

    /**
     *
     */
    #define DOSERR_DATA_INVALID 13

    /**
     *
     */
    #define DOSERR_RESERVED 14

    /**
     *
     */
    #define DOSERR_FIXUP_OVERFLOW 14

    /**
     *
     */
    #define DOSERR_INVALID_DRIVE 15

    /**
     *
     */
    #define DOSERR_REMOVE_CURRENT_DIRECTORY 16

    /**
     *
     */
    #define DOSERR_NOT_SAME_DEVICE 17

    /**
     *
     */
    #define DOSERR_NO_MORE_FILES 18

    /**
     *
     */
    #define DOSERR_FILE_ALREADY_EXISTS 80

    /**
     * Restam algumas classes usadas para acessar
     * certas coisas.
     */

    /**
     *
     */
    #define sOffset(s, m) ((char*)&(((s*)NULL)->m)-(char*)NULL)

    /**
     *
     */
    #define sGet(s, m) GetIt(sizeof(((s *)&pt)->m),(PhysPt)sOffset(s, m))

    /**
     *
     */
    #define sSave(s, m, val) SaveIt(sizeof(((s *)&pt)->m),(PhysPt)sOffset(s, m), val)

    /**
     *
     */
    class MemStruct
    {
        public:
            /**
             *
             */
            Bitu GetIt(Bitu size, PhysPt addr)
            {
                switch (size)
                {
                    case 1:
                        return mem_readb(pt + addr);

                    case 2:
                        return mem_readw(pt + addr);

                    case 4:
                        return mem_readd(pt + addr);
                }

                return 0;
            }

            /**
             *
             */
            void SaveIt(Bitu size, PhysPt addr, Bitu val)
            {
                switch (size)
                {
                    case 1:
                        mem_writeb(pt + addr, (Bit8u)val);
                        break;

                    case 2:
                        mem_writew(pt + addr, (Bit16u)val);
                        break;

                    case 4:
                        mem_writed(pt + addr, (Bit32u)val);
                        break;
                }
            }

            /**
             *
             */
            void SetPt(Bit16u seg)
            {
                pt = PhysMake(seg, 0);
            }

            /**
             *
             */
            void SetPt(Bit16u seg, Bit16u off)
            {
                pt = PhysMake(seg, off);
            }

            /**
             *
             */
            void SetPt(RealPt addr)
            {
                pt = Real2Phys(addr);
            }

        protected:
            /**
             *
             */
            PhysPt pt;
    };

    /**
     *
     */
    class DOS_PSP :public MemStruct
    {
        public:
            /**
             *
             */
            DOS_PSP(Bit16u segment)
            {
                SetPt(segment);
                seg = segment;
            };

            /**
             *
             */
            void MakeNew(Bit16u memSize);

            /**
             *
             */
            void CopyFileTable(DOS_PSP* srcpsp, bool createchildpsp);

            /**
             *
             */
            Bit16u FindFreeFileEntry(void);

            /**
             *
             */
            void CloseFiles(void);

            /**
             *
             */
            void SaveVectors(void);

            /**
             *
             */
            void RestoreVectors(void);

            /**
             *
             */
            void SetSize(Bit16u size)
            {
                sSave(sPSP, next_seg, size);
            };

            /**
             *
             */
            Bit16u GetSize(void)
            {
                return (Bit16u)sGet(sPSP, next_seg);
            };

            /**
             *
             */
            void SetEnvironment(Bit16u envseg)
            {
                sSave(sPSP, environment, envseg);
            };

            /**
             *
             */
            Bit16u GetEnvironment(void)
            {
                return (Bit16u)sGet(sPSP, environment);
            };

            /**
             *
             */
            Bit16u GetSegment(void)
            {
                return seg;
            };

            /**
             *
             */
            void SetFileHandle(Bit16u index, Bit8u handle);

            /**
             *
             */
            Bit8u GetFileHandle(Bit16u index);

            /**
             *
             */
            void SetParent(Bit16u parent)
            {
                sSave(sPSP, psp_parent, parent);
            };

            /**
             *
             */
            Bit16u GetParent(void)
            {
                return (Bit16u)sGet(sPSP, psp_parent);
            };

            /**
             *
             */
            void SetStack(RealPt stackpt)
            {
                sSave(sPSP, stack, stackpt);
            };

            /**
             *
             */
            RealPt GetStack(void)
            {
                return sGet(sPSP, stack);
            };

            /**
             *
             */
            void SetInt22(RealPt int22pt)
            {
                sSave(sPSP, int_22, int22pt);
            };

            /**
             *
             */
            RealPt GetInt22(void)
            {
                return sGet(sPSP, int_22);
            };

            /**
             *
             */
            void SetFCB1(RealPt src);

            /**
             *
             */
            void SetFCB2(RealPt src);

            /**
             *
             */
            void SetCommandTail(RealPt src);

            /**
             *
             */
            bool SetNumFiles(Bit16u fileNum);

            /**
             *
             */
            Bit16u FindEntryByHandle(Bit8u handle);

        private:
            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack(1)
            #endif

            /**
             *
             */
            struct sPSP
            {
                /**
                 * Ponto de saída semelhante ao CP/M.
                 */
                Bit8u exit[2];

                /**
                 * Segmento do primeiro byte além da cubo alocado ou
                 * do programa.
                 */
                Bit16u next_seg;

                /**
                 * Preenchimento de caractere único.
                 */
                Bit8u fill_1;

                /**
                 * Código de operação de chamada distante.
                 */
                Bit8u far_call;

                /**
                 * Endereço de solicitação de serviço CPM.
                 */
                RealPt cpm_entry;

                /**
                 * Terminar Endereço.
                 */
                RealPt int_22;

                /**
                 * Endereço de nova sequência.
                 */
                RealPt int_23;

                /**
                 * Endereço de Erro Crítico.
                 */
                RealPt int_24;

                /**
                 * Segmento PSP de Herança.
                 */
                Bit16u psp_parent;

                /**
                 * Tabela de arquivos - 0xff não é usado.
                 */
                Bit8u files[20];

                /**
                 * Segmento da tabela de ambiente.
                 */
                Bit16u environment;

                /**
                 * SS:SP Salvar ponto para chamadas int 0x21.
                 */
                RealPt stack;

                /**
                 * Máximo de arquivos abertos.
                 */
                Bit16u max_files;

                /**
                 * Ponteiro para a tabela de arquivos PSP:0x18.
                 */
                RealPt file_table;

                /**
                 * Ponteiro para o PSP anterior.
                 */
                RealPt prev_psp;

                /**
                 *
                 */
                Bit8u interim_flag;

                /**
                 *
                 */
                Bit8u truename_flag;

                /**
                 *
                 */
                Bit16u nn_flags;

                /**
                 *
                 */
                Bit16u dos_version;

                /**
                 * Muitas coisas não usadas com as quais não é relevante.
                 */
                Bit8u fill_2[14];

                /**
                 * INT 0x21 Chamada de serviço int 0x21;retf;
                 */
                Bit8u service[3];

                /**
                 * Isso tem alguns blocos com informações do FCB.
                 */
                Bit8u fill_3[9];

                /**
                 * Primeiro FCB.
                 */
                Bit8u fcb1[16];

                /**
                 * Segundo FCB.
                 */
                Bit8u fcb2[16];

                /**
                 * Não utilizado.
                 */
                Bit8u fill_4[4];

                /**
                 *
                 */
                CommandTail cmdtail;
            } GCC_ATTRIBUTE(packed);

            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack()
            #endif

            /**
             *
             */
            Bit16u seg;

        public:
            /**
             *
             */
            static Bit16u rootpsp;
    };

    /**
     *
     */
    class DOS_ParamBlock:public MemStruct
    {
        public:
            /**
             *
             */
            DOS_ParamBlock(PhysPt addr)
            {
                pt = addr;
            }

            /**
             *
             */
            void Clear(void);

            /**
             *
             */
            void LoadData(void);

            /**
             * Salve-o como um bloco proc.
             */
            void SaveData(void);

            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack(1)
            #endif

            /**
             *
             */
            struct sOverlay
            {
                /**
                 *
                 */
                Bit16u loadseg;

                /**
                 *
                 */
                Bit16u relocation;
            } GCC_ATTRIBUTE(packed);

            /**
             *
             */
            struct sExec
            {
                /**
                 *
                 */
                Bit16u envseg;

                /**
                 *
                 */
                RealPt cmdtail;

                /**
                 *
                 */
                RealPt fcb1;

                /**
                 *
                 */
                RealPt fcb2;

                /**
                 *
                 */
                RealPt initsssp;

                /**
                 *
                 */
                RealPt initcsip;
            } GCC_ATTRIBUTE(packed);

            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack()
            #endif

            /**
             *
             */
            sExec exec;

            /**
             *
             */
            sOverlay overlay;
    };

    /**
     *
     */
    class DOS_InfoBlock:public MemStruct
    {
        public:
            /**
             *
             */
            DOS_InfoBlock()
            {
            };

            /**
             *
             */
            void SetLocation(Bit16u seg);

            /**
             *
             */
            void SetFirstMCB(Bit16u _first_mcb);

            /**
             *
             */
            void SetBuffers(Bit16u x, Bit16u y);

            /**
             *
             */
            void SetCurDirStruct(Bit32u _curdirstruct);

            /**
             *
             */
            void SetFCBTable(Bit32u _fcbtable);

            /**
             *
             */
            void SetDeviceChainStart(Bit32u _devchain);

            /**
             *
             */
            void SetDiskBufferHeadPt(Bit32u _dbheadpt);

            /**
             *
             */
            void SetStartOfUMBChain(Bit16u _umbstartseg);

            /**
             *
             */
            void SetUMBChainState(Bit8u _umbchaining);

            /**
             *
             */
            Bit16u GetStartOfUMBChain(void);

            /**
             *
             */
            Bit8u GetUMBChainState(void);

            /**
             *
             */
            RealPt GetPointer(void);

            /**
             *
             */
            Bit32u GetDeviceChain(void);

            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack(1)
            #endif

            /**
             *
             */
            struct sDIB
            {
                /**
                 *
                 */
                Bit8u unknown1[4];

                /**
                 * -0x22 precisa ser 1.
                 */
                Bit16u magicWord;

                /**
                 *
                 */
                Bit8u unknown2[8];

                /**
                 * -0x18 CX do último int21/ah=5e.
                 */
                Bit16u regCXfrom5e;

                /**
                 * -0x16 contador LRU para cache FCB.
                 */
                Bit16u countLRUcache;

                /**
                 * -0x14 contador LRU para aberturas FCB.
                 */
                Bit16u countLRUopens;

                /**
                 * -0x12 algumas coisas, vamos ficar sem usar.
                 */
                Bit8u stuff[6];

                /**
                 * -0x0c contador de tentativas de compartilhamento.
                 */
                Bit16u sharingCount;

                /**
                 * -0x0a atraso na tentativa de compartilhamento.
                 */
                Bit16u sharingDelay;

                /**
                 * -0x08 ponteiro para buffer de disco.
                 */
                RealPt diskBufPtr;

                /**
                 * -0x04 ponteiro para entrada con.
                 */
                Bit16u ptrCONinput;

                /**
                 * -0x02 primeiro bloco de controle de cubo.
                 */
                Bit16u firstMCB;

                /**
                 * 0x00 primeiro bloco de parâmetros de acionamento.
                 */
                RealPt firstDPB;

                /**
                 * 0x04 primeira tabela de arquivos do sistema.
                 */
                RealPt firstFileTable;

                /**
                 * 0x08 cabeçalho de dispositivo de relógio ativo.
                 */
                RealPt activeClock;

                /**
                 * 0x0c cabeçalho de dispositivo de console ativo.
                 */
                RealPt activeCon;

                /**
                 * 0x10 bytes máximos por setor de qualquer dispositivo
                 * de bloco;
                 */
                Bit16u maxSectorLength;

                /**
                 * 0x12 ponteiro para o buffer de informações do disco.
                 */
                RealPt diskInfoBuffer;

                /**
                 * 0x16 ponteiro para o array atual da estrutura de
                 * diretórios.
                 */
                RealPt curDirStructure;

                /**
                 * 0x1a ponteiro para a tabela FCB do sistema.
                 */
                RealPt fcbTable;

                /**
                 * 0x1e fcbs protegido.
                 */
                Bit16u protFCBs;

                /**
                 * 0x20 dispositivos de bloco instalados.
                 */
                Bit8u blockDevices;

                /**
                 * 0x21 última unidade.
                 */
                Bit8u lastdrive;

                /**
                 * 0x22 NUL driver próximo ponteiro.
                 */
                Bit32u nulNextDriver;

                /**
                 * 0x26 NUL driver atributos.
                 */
                Bit16u nulAttributes;

                /**
                 * Rotina de estratégia de driver 0x28 NUL.
                 */
                Bit32u nulStrategy;

                /**
                 * 0x2c NUL string do nome do driver.
                 */
                Bit8u nulString[8];

                /**
                 * Unidades unidas 0x34.
                 */
                Bit8u joindedDrives;

                /**
                 * Segmento de código especial 0x35.
                 */
                Bit16u specialCodeSeg;

                /**
                 * 0x37 ponteiro para setver.
                 */
                RealPt setverPtr;

                /**
                 * 0x3b a20 corrigir offset de rotina.
                 */
                Bit16u a20FixOfs;

                /**
                 * 0x3d psp do último programa (se for DOS em hma).
                 */
                Bit16u pspLastIfHMA;

                /**
                 * 0x3f x em BUFFERS x,y.
                 */
                Bit16u buffers_x;

                /**
                 * 0x41 y em BUFFERS x,y
                 */
                Bit16u buffers_y;

                /**
                 * Unidade de inicialização 0x43.
                 */
                Bit8u bootDrive;

                /**
                 * 0x44 usa movimentos dword.
                 */
                Bit8u useDwordMov;

                /**
                 * 0x45 tamanho do cubo estendido.
                 */
                Bit16u extendedSize;

                /**
                 * 0x47 ponteiro para o cabeçalho do buffer menos
                 * usado recentemente.
                 */
                Bit32u diskBufferHeadPt;

                /**
                 * 0x4b número de buffers de disco que não estão seguindo
                 * uma especificação.
                 */
                Bit16u dirtyDiskBuffers;

                /**
                 * Ponteiro 0x4d para buffer de antecipação.
                 */
                Bit32u lookaheadBufPt;

                /**
                 * 0x51 número de buffers de antecipação.
                 */
                Bit16u lookaheadBufNumber;

                /**
                 * Localização do buffer do espaço de trabalho 0x53.
                 */
                Bit8u bufferLocation;

                /**
                 * 0x54 ponteiro para o buffer do espaço de trabalho.
                 */
                Bit32u workspaceBuffer;

                /**
                 * 0x58.
                 */
                Bit8u unknown3[11];

                /**
                 * 0x63 bit0: sequência UMB vinculada à sequência MCB.
                 */
                Bit8u chainingUMB;

                /**
                 * 0x64 parágrafos mínimos necessários para o programa atual.
                 */
                Bit16u minMemForExec;

                /**
                 * Segmento 0x66 do primeiro UMB-MCB.
                 */
                Bit16u startOfUMBChain;

                /**
                 * 0x68 parágrafo inicial para alocação de cubo.
                 */
                Bit16u memAllocScanStart;
            } GCC_ATTRIBUTE(packed);

            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack()
            #endif

            /**
             *
             */
            Bit16u seg;
    };

    /**
     *
     */
    class DOS_DTA: public MemStruct
    {
        public:
            /**
             *
             */
            DOS_DTA(RealPt addr)
            {
                SetPt(addr);
            }

            /**
             *
             */
            void SetupSearch(Bit8u _sdrive, Bit8u _sattr, char * _pattern);

            /**
             *
             */
            void SetResult(const char * _name, Bit32u _size, Bit16u _date, Bit16u _time, Bit8u _attr);

            /**
             *
             */
            Bit8u GetSearchDrive(void);

            /**
             *
             */
            void GetSearchParams(Bit8u & _sattr, char * _spattern);

            /**
             *
             */
            void GetResult(char * _name, Bit32u & _size, Bit16u & _date, Bit16u & _time, Bit8u & _attr);

            /**
             *
             */
            void SetDirID(Bit16u entry)
            {
                sSave(sDTA, dirID, entry);
            };

            /**
             *
             */
            void SetDirIDCluster(Bit16u entry)
            {
                sSave(sDTA, dirCluster, entry);
            };

            /**
             *
             */
            Bit16u GetDirID(void)
            {
                return (Bit16u)sGet(sDTA, dirID);
            };

            /**
             *
             */
            Bit16u GetDirIDCluster(void)
            {
                return (Bit16u)sGet(sDTA, dirCluster);
            };

        private:
            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack(1)
            #endif

            /**
             *
             */
            struct sDTA
            {
                /**
                 * No Drive a busca está ocorrendo.
                 */
                Bit8u sdrive;

                /**
                 * O padrão de pesquisa para o nome do arquivo.
                 */
                Bit8u sname[8];

                /**
                 * O padrão de pesquisa para a extensão.
                 */
                Bit8u sext[3];

                /**
                 * Os atributos que precisam ser encontrados.
                 */
                Bit8u sattr;

                /**
                 * Custom: dir-search ID para várias pesquisas ao mesmo tempo.
                 */
                Bit16u dirID;

                /**
                 * Personalizado (somente drive_fat): número do cluster
                 * para várias pesquisas ao mesmo tempo.
                 */
                Bit16u dirCluster;

                /**
                 *
                 */
                Bit8u fill[4];

                /**
                 *
                 */
                Bit8u attr;

                /**
                 *
                 */
                Bit16u time;

                /**
                 *
                 */
                Bit16u date;

                /**
                 *
                 */
                Bit32u size;

                /**
                 *
                 */
                char name[DOS_NAMELENGTH_ASCII];
            } GCC_ATTRIBUTE(packed);

            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack()
            #endif
    };

    /**
     *
     */
    class DOS_FCB: public MemStruct
    {
        public:
            /**
             *
             */
            DOS_FCB(Bit16u seg, Bit16u off, bool allow_extended = true);

            /**
             *
             */
            void Create(bool _extended);

            /**
             *
             */
            void SetName(Bit8u _drive, char * _fname, char * _ext);

            /**
             *
             */
            void SetSizeDateTime(Bit32u _size, Bit16u _date, Bit16u _time);

            /**
             *
             */
            void GetSizeDateTime(Bit32u & _size, Bit16u & _date, Bit16u & _time);

            /**
             *
             */
            void GetName(char * fillname);

            /**
             *
             */
            void FileOpen(Bit8u _fhandle);

            /**
             *
             */
            void FileClose(Bit8u & _fhandle);

            /**
             *
             */
            void GetRecord(Bit16u & _cur_block, Bit8u & _cur_rec);

            /**
             *
             */
            void SetRecord(Bit16u _cur_block, Bit8u _cur_rec);

            /**
             *
             */
            void GetSeqData(Bit8u & _fhandle, Bit16u & _rec_size);

            /**
             *
             */
            void GetRandom(Bit32u & _random);

            /**
             *
             */
            void SetRandom(Bit32u _random);

            /**
             *
             */
            Bit8u GetDrive(void);

            /**
             *
             */
            bool Extended(void);

            /**
             *
             */
            void GetAttr(Bit8u & attr);

            /**
             *
             */
            void SetAttr(Bit8u attr);

            /**
             *
             */
            bool Valid(void);

        private:
            /**
             *
             */
            bool extended;

            /**
             *
             */
            PhysPt real_pt;

            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack(1)
            #endif

            /**
             *
             */
            struct sFCB
            {
                /**
                 * Número de drive: 0=default, 1=A, etc.
                 */
                Bit8u drive;

                /**
                 * Nome preenchido com espaço.
                 */
                Bit8u filename[8];

                /**
                 * Extensão acolchoada com espaço.
                 */
                Bit8u ext[3];

                /**
                 * Bloco atual.
                 */
                Bit16u cur_block;

                /**
                 * Tamanho do registro lógico.
                 */
                Bit16u rec_size;

                /**
                 * Tamanho do arquivo.
                 */
                Bit32u filesize;

                /**
                 *
                 */
                Bit16u date;

                /**
                 *
                 */
                Bit16u time;

                /**
                 * Bloco reservado deve ter 8 bytes.
                 */

                /**
                 *
                 */
                Bit8u sft_entries;

                /**
                 *
                 */
                Bit8u share_attributes;

                /**
                 *
                 */
                Bit8u extra_info;

                /**
                 *
                 */
                Bit8u file_handle;

                /**
                 *
                 */
                Bit8u reserved[4];

                /**
                 * Fim.
                 */

                /**
                 * Registro atual no bloco atual.
                 */
                Bit8u cur_rec;

                /**
                 * Número de registro relativo atual.
                 */
                Bit32u rndm;
            } GCC_ATTRIBUTE(packed);

            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack()
            #endif
    };

    /**
     *
     */
    class DOS_MCB : public MemStruct
    {
        public:
            /**
             *
             */
            DOS_MCB(Bit16u seg)
            {
                SetPt(seg);
            }

            /**
             *
             */
            void SetFileName(char const * const _name)
            {
                MEM_BlockWrite(pt + offsetof(sMCB, filename), _name, 8);
            }

            /**
             *
             */
            void GetFileName(char * const _name)
            {
                MEM_BlockRead(pt + offsetof(sMCB, filename), _name,8);
                _name[8] = 0;
            }

            /**
             *
             */
            void SetType(Bit8u _type)
            {
                sSave(sMCB, type, _type);
            }

            /**
             *
             */
            void SetSize(Bit16u _size)
            {
                sSave(sMCB, size, _size);
            }

            /**
             *
             */
            void SetPSPSeg(Bit16u _pspseg)
            {
                sSave(sMCB, psp_segment, _pspseg);
            }

            /**
             *
             */
            Bit8u GetType(void)
            {
                return (Bit8u)sGet(sMCB, type);
            }

            /**
             *
             */
            Bit16u GetSize(void)
            {
                return (Bit16u)sGet(sMCB, size);
            }

            /**
             *
             */
            Bit16u GetPSPSeg(void)
            {
                return (Bit16u)sGet(sMCB, psp_segment);
            }

        private:
            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack(1)
            #endif

            /**
             *
             */
            struct sMCB
            {
                /**
                 *
                 */
                Bit8u type;

                /**
                 *
                 */
                Bit16u psp_segment;

                /**
                 *
                 */
                Bit16u size;

                /**
                 *
                 */
                Bit8u unused[3];

                /**
                 *
                 */
                Bit8u filename[8];
            } GCC_ATTRIBUTE(packed);

            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack()
            #endif
    };

    /**
     *
     */
    class DOS_SDA : public MemStruct
    {
        public:
            /**
             *
             */
            DOS_SDA(Bit16u _seg, Bit16u _offs)
            {
                SetPt(_seg, _offs);
            }

            /**
             *
             */
            void Init();

            /**
             *
             */
            void SetDrive(Bit8u _drive)
            {
                sSave(sSDA, current_drive, _drive);
            }

            /**
             *
             */
            void SetDTA(Bit32u _dta)
            {
                sSave(sSDA, current_dta, _dta);
            }

            /**
             *
             */
            void SetPSP(Bit16u _psp)
            {
                sSave(sSDA, current_psp, _psp);
            }

            /**
             *
             */
            Bit8u GetDrive(void)
            {
                return (Bit8u)sGet(sSDA, current_drive);
            }

            /**
             *
             */
            Bit16u GetPSP(void)
            {
                return (Bit16u)sGet(sSDA, current_psp);
            }

            /**
             *
             */
            Bit32u GetDTA(void)
            {
                return (Bit32u)sGet(sSDA, current_dta);
            }

        private:
            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack(1)
            #endif

            /**
             *
             */
            struct sSDA
            {
                /**
                 * 0x00 Sinalizador de erro crítico.
                 */
                Bit8u crit_error_flag;

                /**
                 * 0x01 sinalizador InDOS (contador de chamadas
                 * INT 21 ativas).
                 */
                Bit8u inDOS_flag;

                /**
                 * 0x02 Drive no qual ocorreu a falha crítica atual ou FFh.
                 */
                Bit8u drive_crit_error;

                /**
                 * 0x03 locus da última falha.
                 */
                Bit8u locus_of_last_error;

                /**
                 * 0x04 código de falha estendida da última falha.
                 */
                Bit16u extended_error_code;

                /**
                 * 0x06 ação sugerida para a última falha.
                 */
                Bit8u suggested_action;

                /**
                 * 0x07 classe da última falha.
                 */
                Bit8u error_class;

                /**
                 * 0x08 ES:DI ponteiro para o último erro.
                 */
                Bit32u last_error_pointer;

                /**
                 * 0x0C DTA atual (Endereço de transferência de disco).
                 */
                Bit32u current_dta;

                /**
                 * 0x10 PSP atual.
                 */
                Bit16u current_psp;

                /**
                 * 0x12 armazena SP em um INT 23.
                 */
                Bit16u sp_int_23;

                /**
                 * 0x14 código de retorno da última finalização do
                 * processo (zero após leitura com AH=4Dh).
                 */
                Bit16u return_code;

                /**
                 * 0x16 drive atual.
                 */
                Bit8u current_drive;

                /**
                 * 0x17 bandeira de pausa estendida.
                 */
                Bit8u extended_break_flag;

                /**
                 * Sinalizador 0x18: troca de página de código || Sinalizador:
                 * cópia do byte anterior em caso de terminar INT 24.
                 */
                Bit8u fill[2];
            } GCC_ATTRIBUTE(packed);

            /**
             *
             */
            #ifdef _MSC_VER
                /**
                 *
                 */
                #pragma pack()
            #endif
    };

    /**
     *
     */
    extern DOS_InfoBlock dos_infoblock;

    /**
     *
     */
    struct DOS_Block
    {
        /**
         *
         */
        DOS_Date date;

        /**
         *
         */
        DOS_Version version;

        /**
         *
         */
        Bit16u firstMCB;

        /**
         *
         */
        Bit16u errorcode;

        /**
         *
         */
        Bit16u psp()
        {
            return DOS_SDA(DOS_SDA_SEG, DOS_SDA_OFS).GetPSP();
        };

        /**
         *
         */
        void psp(Bit16u _seg)
        {
            DOS_SDA(DOS_SDA_SEG, DOS_SDA_OFS).SetPSP(_seg);
        };

        /**
         *
         */
        Bit16u env;

        /**
         *
         */
        RealPt cpmentry;

        /**
         *
         */
        RealPt dta()
        {
            return DOS_SDA(DOS_SDA_SEG, DOS_SDA_OFS).GetDTA();
        };

        /**
         *
         */
        void dta(RealPt _dta)
        {
            DOS_SDA(DOS_SDA_SEG, DOS_SDA_OFS).SetDTA(_dta);
        };

        /**
         *
         */
        Bit8u return_code,return_mode;

        /**
         *
         */
        Bit8u current_drive;

        /**
         *
         */
        bool verify;

        /**
         *
         */
        bool breakcheck;

        /**
         * Se definido como true dev_con::read irá ecoar
         * a entrada.
         */
        bool echo;

        /**
         *
         */
        struct
        {
            /**
             *
             */
            RealPt mediaid;

            /**
             *
             */
            RealPt tempdta;

            /**
             *
             */
            RealPt tempdta_fcbdelete;

            /**
             *
             */
            RealPt dbcs;

            /**
             *
             */
            RealPt filenamechar;

            /**
             *
             */
            RealPt collatingseq;

            /**
             *
             */
            RealPt upcase;

            /**
             * Será copiado para o cubo DOS. reside no cubo.
             */
            Bit8u* country;

            /**
             * Sistema de parâmetro Fake Disk usando apenas a primeira
             * entrada para que a letra da unidade corresponda.
             */
            Bit16u dpb;
        } tables;

        /**
         *
         */
        Bit16u loaded_codepage;
    };

    /**
     *
     */
    extern DOS_Block dos;

    /**
     *
     */
    static INLINE Bit8u RealHandle(Bit16u handle)
    {
        DOS_PSP psp(dos.psp());

        return psp.GetFileHandle(handle);
    }

#endif
