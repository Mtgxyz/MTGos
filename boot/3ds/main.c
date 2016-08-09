#include "common.h"
#include "sdmmc.h"
#include "fatfs/ff.h"
char *lfb=(char*)0x18346500;
#define DIAGPXL(i) (lfb[6*(i)]=lfb[6*(i)+1]=lfb[6*(i)+2]=0xFF)
/**
 * \struct FIRM_sect
 * \brief Contains one section of the FIRM format
 */
struct FIRM_sect {
    unsigned int offset; //! Offset in file (bytes)
    unsigned int physical; //! Physical address, where the section is copied to
    unsigned int size; //! Size of section
    unsigned int arm11; //! currently unused
    unsigned char SHA256[0x20]; //! Currently unused
}__attribute__((packed));
/**
 * \struct FIRM_header
 * \brief Contains the first sector of every FIRM file.
 */
struct FIRM_header {
    char magic[4]; //! Magic "FIRM" string (not-null terminated)
    int version; //! Version. Currently 1
    void(*entrypoint)(void**); //! Address where the processor jumps to after loading
    void (*arm11entry)(void**);
    unsigned int reserved[0xC];
    struct FIRM_sect sections[4]; //! The four internal sections
    unsigned char RSA2048[0x100]; //! Currently unused
}__attribute__((packed));
/**
 * \brief Module table for the kernel (Up to 1024 mods)
 */
void *arm9modtable[64];
void *arm11modtable[64];
struct FIRM_header hdr;
void init() {
    DIAGPXL(0);
    FATFS fs;
    FIL firm;
    f_mount(&fs, "0:", 0);
    arm9modtable[0]=0x20000000;
    arm9modtable[1]=0;
    arm11modtable[0]=0;
    if(f_open(&firm, "mtgos.firm", FA_READ | FA_OPEN_EXISTING) == FR_OK) {
        DIAGPXL(1);
        unsigned int br;
        f_read(&firm, (void*)&hdr, sizeof(struct FIRM_header), &br);
        DIAGPXL(2);
        if((hdr.magic[0]!='F')||(hdr.magic[1]!='I')||(hdr.magic[2]!='R')||(hdr.magic[3]!='M'))
            while(1);
        DIAGPXL(3);
        for(int i=0;i<4;i++) { //Load sections
            if(hdr.sections[i].size==0)
                continue;
            f_lseek(&firm, hdr.sections[i].offset);
            *((char*)hdr.sections[i].physical)=0xFF;
            char oldval=0xFF;
            f_read(&firm, (void*)hdr.sections[i].physical, hdr.sections[i].size, &br);
            if(oldval!=*((char*)hdr.sections[i].physical))
                DIAGPXL(i+4);
            DIAGPXL(i+8);
        }
        DIAGPXL(12);
        FIL dsp_txt;
        f_open(&dsp_txt, "dsp_txt.elf", FA_READ | FA_OPEN_EXISTING);
        f_read(&dsp_txt, (void*)0x20000000, f_size(&dsp_txt), &br);
        void(**a11fpointer)(void**)=(void(**)(void**))0x1FFFFFF8;
        *a11fpointer=hdr.arm11entry;
        hdr.entrypoint(arm9modtable); //Jump to kernel
    }
    for(;;);
}