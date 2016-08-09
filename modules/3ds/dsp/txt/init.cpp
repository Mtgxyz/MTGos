#include <moduleinterface.h>
#include <base/output.hpp>
#include <modstubs.h>
#include "stdfnt.h"
uint8_t* vmem = (uint8_t*)0x18300000;
uint8_t* dmem = (uint8_t*)0x18346500;
static int x=0,y=0;
#define DIAGPXL(i) (dmem[6*(i)]=dmem[6*(i)+1]=dmem[6*(i)+2]=0xFF)
#define CHR_HEIGHT 8
#define CHR_WIDTH 8
#define HEIGHT 29
#define WIDTH 50
#define BYTESPP 3
void debugNumber(unsigned int i, int start) {
    while(i) {
        if(i&1)
            dmem[(start*6)+5]=0xFF;
        i>>=1;
        dmem[(start*6)+1]=0x50;
        start++;
    }
}
namespace MTGos {
namespace {
/**
 * \class Screen
 * \brief Text output to the screen
 */
class Screen: public Base::Output {
public:
    Screen() {DIAGPXL(31);}
    auto scroll() -> void {
        for(int p=0;p<HEIGHT*WIDTH*CHR_HEIGHT*CHR_WIDTH*BYTESPP;p++)
            vmem[p]=vmem[p+WIDTH*CHR_WIDTH*BYTESPP];
        for(int p=HEIGHT*WIDTH*CHR_HEIGHT*CHR_WIDTH*BYTESPP; p<(HEIGHT*WIDTH+WIDTH)*CHR_HEIGHT*CHR_WIDTH*BYTESPP; p++)
            vmem[p]=0x00;
        y--;
    }
private:
    virtual auto putChar(int c) -> void{
        c&=0xFF;
        debugNumber((unsigned int)c,500);
        for(int i=0;i<8;i++) {
            debugNumber((unsigned int)font[c][i],532+8*i);
        }
        debugNumber((unsigned int)x,532+8*8);
        debugNumber((unsigned int)y,532+8*8+32);
        switch(c) {
        case '\n':
            x=0; y++;
            break;
        case '\r':
            x=0;
            break;
        case '\b':
            if(x==0)
                break;
            x--;
            putChar(' '),
            x--;
            break;
        case '\0':
            DIAGPXL(32);
            break;
        default:
            for(int cx=0;cx<CHR_WIDTH;cx++) {
                for(int cy=0;cy<CHR_HEIGHT;cy++) {
                    if(font[c][cy]&(1<<cx)) {
                        vmem[((x*CHR_WIDTH+cx)*(HEIGHT+1)*8+((HEIGHT-y)*CHR_HEIGHT+cy))*BYTESPP]=0xFF;
                        vmem[((x*CHR_WIDTH+cx)*(HEIGHT+1)*8+((HEIGHT-y)*CHR_HEIGHT+cy))*BYTESPP+1]=0;
                    }else {
                        vmem[((x*CHR_WIDTH+cx)*(HEIGHT+1)*8+((HEIGHT-y)*CHR_HEIGHT+cy))*BYTESPP+1]=0xFF;
                        vmem[((x*CHR_WIDTH+cx)*(HEIGHT+1)*8+((HEIGHT-y)*CHR_HEIGHT+cy))*BYTESPP]=0;
                    }
                }
            }
            x++;
            if(x>WIDTH) {
                x=0;
                y++;
            }
            break;
        }
        debugNumber((unsigned int)y,532+8*8+64);
        //if(y>HEIGHT)
        //    scroll();
    }
};
}
}
void(*tbl[3])()={(void(*)())&getType,(void(*)())&size_of,(void(*)())&spawnAt};
table_type getTable() {
    x=y=0;
    doCtors();
    DIAGPXL(23);
    return (table_type)&tbl;
}
auto getType() -> ModType {
    DIAGPXL(24);
    return ModType::output_text;
}
auto spawnAt(void* pos) -> bool {
    debugNumber((unsigned int)pos,82);
    DIAGPXL(30);
    new(pos) MTGos::Screen;
    return true;
}
auto size_of() -> size_t {
    DIAGPXL(26);
    return sizeof(MTGos::Screen);
}