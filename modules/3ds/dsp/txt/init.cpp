#include <moduleinterface.h>
#include <base/output.hpp>
#include <modstubs.h>
int x=0,y=0;
uint8_t* vmem = (uint8_t*)0x18300000;
#define DIAGPXL(i) (vmem[6*(i)]=vmem[6*(i)+1]=vmem[6*(i)+2]=0xFF)
void(*tbl[3])()={(void(*)())&getType,(void(*)())&size_of,(void(*)())&spawnAt};
table_type getTable() {
    doCtors();
    DIAGPXL(23);
    return (table_type)&tbl;
}
auto getType() -> ModType {
    DIAGPXL(24);
    return ModType::output_text;
}
void debugNumber(unsigned int i, int start) {
    while(i) {
        if(i&1)
            vmem[(start*6)+5]=0xFF;
        i>>=1;
        vmem[(start*6)+1]=0x50;
        start++;
    }
}
auto spawnAt(void* pos) -> bool {
    debugNumber((unsigned int)pos,82);
    DIAGPXL(25);
    return false;
    //new(pos) MTGos::Screen;
    return true;
}
auto size_of() -> size_t {
    DIAGPXL(26);
    return 0;//sizeof(MTGos::Screen);
}