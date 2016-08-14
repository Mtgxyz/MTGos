#include <stdint.h>
#include <moduleinterface.h>
#include <elf.h>
#include <base/output.hpp>
#ifdef __LP64__
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#else
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Phdr Elf32_Phdr
#endif
char *lfb=(char*)0x18346500;
#ifndef ARM9
#define DIAGPXL(i) (lfb[6*(i)]=lfb[6*(i)+1]=lfb[6*(i)+2]=0xFF)
#else
#define DIAGPXL(i) (0)
#endif
/** \brief beginning of constructor table */
extern "C" void(*start_ctors)(); 
/** \brief end of constructor table */
extern "C" void(*end_ctors)();
/** \brief start of destructor table */
extern "C" void(*start_dtors)();
/** \brief end of destructor table */
extern "C" void(*end_dtors)();
/**
 * \function load(Elf32_Ehdr)
 * \brief returns program entry point
 */
auto load(Elf_Ehdr* file) -> void(**(*)(void*))() {
    if((file->e_ident[0]!=ELFMAG0)||
      (file->e_ident[1]!=ELFMAG1)||
      (file->e_ident[2]!=ELFMAG2)||
      (file->e_ident[3]!=ELFMAG3) ) {
        return nullptr;
    }
    Elf_Phdr *phdr = (Elf_Phdr*)((uintptr_t)(file->e_phoff)+(uintptr_t)file);
    for(int i=0;i<file->e_phnum;i++) {
        uintptr_t start=phdr[i].p_vaddr;
        uintptr_t end=start+phdr[i].p_memsz;
        if((start <= file->e_entry) && (file->e_entry < end)) {
            return (void (** (*)(void*))()) (file->e_entry-start+phdr[i].p_offset+(uintptr_t)file); //Calculate _start address
        }
    }
  return nullptr;
}
MTGos::Base::Output out;
void debugNumber(uintptr_t i, int start) {
    while(i) {
        if(i&1)
            lfb[(start*6)+5]=0xFF;
        i>>=1;
        lfb[(start*6)+1]=0x50;
        start++;
    }
}
void adjustVTable(uintptr_t** obj, uintptr_t mod, int vtableSize) {
    mod+=0x1000;
    for(int i=0;i<vtableSize;i++) {
        (*obj)[i]+=mod;
    }
}
/**
 * \function _start()
 * \brief Initializes the kernel
 */
extern "C" void _start(void ** modtable) {
    DIAGPXL(13);
    //for(void(**i)()=&start_ctors;i<&end_ctors;i++)
    //    (*i)(); //Calling constructors
    for(int i=0;i<1024;i++) {
        DIAGPXL(14);
        if(!modtable[i])
            break;
        DIAGPXL(15);
        void(**(*fptr)(void*))() = load((Elf_Ehdr*) modtable[i]);
        if(!fptr)
            continue;
//        fptr=(void(**(*)(void*))())((uintptr_t)fptr-8);
        DIAGPXL(16);
        debugNumber((uintptr_t)fptr,50);
//        debugNumber((uintptr_t)modtable[i],50+32);
        DIAGPXL(17);
        void(**table)()=fptr(modtable[i]);
        DIAGPXL(18);
#ifndef __LP64__
        //Relocate table
        table=(void(**)())((uintptr_t)table+(uintptr_t)modtable[i]+0x1000);
#endif
        //Relocate table contents
        uintptr_t* tbl=(uintptr_t*)table;
        tbl[0]+=(uintptr_t)modtable[i]+0x1000;
        tbl[1]+=(uintptr_t)modtable[i]+0x1000;
        tbl[2]+=(uintptr_t)modtable[i]+0x1000;
        debugNumber((uintptr_t)table[0],50+32);
        ModType type=((getType_type)table[0])(); //Get module type
        DIAGPXL(19);
        if(type!=ModType::output_text)
            continue;
        DIAGPXL(20);
        size_t size=((sizeof_type)table[1])(); //Get module size
        DIAGPXL(21);
        ((spawnAt_type)table[2])((void*)&out);
        DIAGPXL(22);
        adjustVTable((uintptr_t**) &out, (uintptr_t)modtable[i], 1);
        out << "HI!\nbye!\n";
#ifdef ARM9
        out << "Here arm9!\n";  
#else
        out << "Here arm11!\n";
#endif
        char* x=(char*)0xB80000;
        x[0]='H';
    }
    for(void(**i)()=&start_dtors;i<&end_dtors;i++)
        (*i)(); //Calling destructors
    for(;;);
}
/**
 * \function __cxa_pure_virtual()
 * \brief Called when a function tries to call a pure-virtual function
 */
extern "C" void __cxa_pure_virtual()
{
    // Do nothing or print an error message.
}
