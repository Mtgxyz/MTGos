// firmloader for x86
#include "multiboot.h"
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
    unsigned int reserved[0xD];
    struct FIRM_sect sections[4]; //! The four internal sections
    unsigned char RSA2048[0x100]; //! Currently unused
}__attribute__((packed));
/**
 * \brief Module table for the kernel (Up to 1024 mods)
 */
void *modtable[1024];
/**
 * \fn init(int, struct multiboot_info*)
 * \brief This routine is called by boot.S
 * This routine is called by boot.S.
 * It loads and jumps to a FIRM binary.
 */
void init(int eax, struct multiboot_info* mb_info) {
    multiboot_module_t *mods = (multiboot_module_t *) mb_info->mods_addr;
    for(unsigned int i=0;i<mb_info->mods_count;i++) { //Get modules
        modtable[i]=(void*)mods[i].mod_start;
    }
    modtable[mb_info->mods_count]=0;
    for(unsigned int i=0;i<mb_info->mods_count;i++) {
        struct FIRM_header *firm=(struct FIRM_header*)(mods[i].mod_start);
        if((firm->magic[0]!='F')||(firm->magic[1]!='I')||(firm->magic[2]!='R')||(firm->magic[3]!='M'))
            continue;
        //We found a FIRM!
        for(int j=0;j<4;j++) {
            if(!firm->sections[j].size)
                continue;
            unsigned char* from=(char*)(mods[i].mod_start+firm->sections[j].offset);
            unsigned char* to=(char*)(firm->sections[j].physical);
            unsigned int size=firm->sections[j].size;
            while(size--) {
                *to++=*from++;
            }
        }
        firm->entrypoint(modtable); //Jump to kernel
    }
}