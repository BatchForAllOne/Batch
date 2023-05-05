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


#ifndef DOSBOX_BIOS_DISK_H
#define DOSBOX_BIOS_DISK_H

    #include <stdio.h>

    #ifndef DOSBOX_MEM_H
    #include "mem.h"
    #endif

    #ifndef DOSBOX_DOS_INC_H
    #include "dos_inc.h"
    #endif

    #ifndef DOSBOX_BIOS_H
    #include "bios.h"
    #endif

    /**
     * A seção que trata do acesso ao disco do BIOS.
     */
    #define BIOS_MAX_DISK 10

    /**
     *
     */
    #define MAX_SWAPPABLE_DISKS 20

    /**
     *
     */
    struct diskGeo
    {
        /**
         * Tamanho em kilobytes.
         */
        Bit32u ksize;

        /**
         * Setores por trilha.
         */
        Bit16u secttrack;

        /**
         * Cabeçalhos por cilindro.
         */
        Bit16u headscyl;

        /**
         * Cilindros por lado.
         */
        Bit16u cylcount;

        /**
         * Digite para retornar do BIOS.
         */
        Bit16u biosval;
    };

    /**
     *
     */
    extern diskGeo DiskGeometryList[];

    /**
     *
     */
    class imageDisk
    {
        public:
            /**
             *
             */
            Bit8u Read_Sector(Bit32u head, Bit32u cylinder, Bit32u sector, void * data);

            /**
             *
             */
            Bit8u Write_Sector(Bit32u head, Bit32u cylinder, Bit32u sector, void * data);

            /**
             *
             */
            Bit8u Read_AbsoluteSector(Bit32u sectnum, void * data);

            /**
             *
             */
            Bit8u Write_AbsoluteSector(Bit32u sectnum, void * data);

            /**
             *
             */
            void Set_Geometry(Bit32u setHeads, Bit32u setCyl, Bit32u setSect, Bit32u setSectSize);

            /**
             *
             */
            void Get_Geometry(Bit32u * getHeads, Bit32u *getCyl, Bit32u *getSect, Bit32u *getSectSize);

            /**
             *
             */
            Bit8u GetBiosType(void);

            /**
             *
             */
            Bit32u getSectSize(void);

            /**
             *
             */
            imageDisk(FILE *imgFile, Bit8u *imgName, Bit32u imgSizeK, bool isHardDisk);

            /**
             *
             */
            ~imageDisk()
            {
                if(diskimg != NULL)
                {
                    fclose(diskimg);
                }
            };

            /**
             *
             */
            bool hardDrive;

            /**
             *
             */
            bool active;

            /**
             *
             */
            FILE *diskimg;

            /**
             *
             */
            Bit8u diskname[512];

            /**
             *
             */
            Bit8u floppytype;

            /**
             *
             */
            Bit32u sector_size;

            /**
             *
             */
            Bit32u heads, cylinders, sectors;
    };

    /**
     *
     */
    void updateDPT(void);

    /**
     *
     */
    #define MAX_HDD_IMAGES 2

    /**
     *
     */
    #define MAX_DISK_IMAGES (2 + MAX_HDD_IMAGES)

    /**
     *
     */
    extern imageDisk *imageDiskList[MAX_DISK_IMAGES];

    /**
     *
     */
    extern imageDisk *diskSwap[MAX_SWAPPABLE_DISKS];

    /**
     *
     */
    extern Bits swapPosition;

    /**
     * Localização do cubo do ponteiro DTA temporário para
     * acesso ao disco de imagem maior.
     */
    extern Bit16u imgDTASeg;

    /**
     * Localização do cubo real do ponteiro DTA temporário
     * para acesso ao disco de imagem maior.
     */
    extern RealPt imgDTAPtr;

    /**
     *
     */
    extern DOS_DTA *imgDTA;

    /**
     *
     */
    void swapInDisks(void);

    /**
     *
     */
    void swapInNextDisk(void);

    /**
     *
     */
    bool getSwapRequest(void);

#endif
