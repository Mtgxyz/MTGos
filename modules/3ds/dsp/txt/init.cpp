#include <moduleinterface.h>
#include <base/output.hpp>
#include <modstubs.h>
#include "stdfnt.h"
#ifndef ARM9
uint8_t* vmem = (uint8_t*)0x18300000;
uint8_t* dmem = (uint8_t*)0x18346500;
#else
uint8_t* dmem = (uint8_t*)0x18300000;
uint8_t* vmem = (uint8_t*)0x18346500;
#endif
static int x=0,y=0;
#define CHR_HEIGHT 8
#define CHR_WIDTH 8
#define HEIGHT 29
#define PXHEIGHT 240
#ifndef ARM9
#define WIDTH 50
#define PXWIDTH 400
#else
#define WIDTH 40
#define PXWIDTH 320
#endif
#define BYTESPP 3
#define CALCXY(x,y) ((x)*240+239-(y))
#define TOPLFB 0x18000000
#define BOTTOMLFB 0x18180000
#ifdef ARM9
#define LFB BOTTOMLFB
#else
#define LFB TOPLFB
#endif
namespace MTGos {
namespace {
/**
 * \class Screen
 * \brief Text output to the screen
 */
class Screen: public Base::Output {
public:
    auto clrscr() -> void {
        uint32_t* vmem = (uint32_t*)LFB;
        for(int i=0;i<PXWIDTH*PXHEIGHT;i++) {
            vmem[i]=0;
        }
    }
    Screen() {
#ifdef ARM11
        // On ARM11, activate screens first
        //top screen lfb size: 0x5dc00
        //bottom screen lfb size: 0x4b000
        //lfb locations: 0x18000000 (left 1)
        //lfb locations: 0x18060000 (left 2)
        //lfb locations: 0x180C0000 (right 1)
        //lfb locations: 0x18120000 (right 2)
        //lfb locations: 0x18180000 (bottom 1)
        //lfb locations: 0x181D0000 (bottom 2)
        unsigned int *lfbs = (unsigned int*)0x18000000;
        unsigned int *fb_init=(unsigned int*)0x10400400;
        fb_init[0x70/4]&=~0x7;
        fb_init[0x170/4]&=~0x7;
        fb_init[0x68/4]=0x18000000; //Left eye
        fb_init[0x6C/4]=0x18060000; //Left eye
        fb_init[0x90/4]=960;
        fb_init[0x94/4]=0x180C0000; //Right eye
        fb_init[0x98/4]=0x18120000; //Right eye
        fb_init[0x168/4]=0x18180000;
        fb_init[0x16C/4]=0x181D0000;
        fb_init[0x190/4]=960;
#endif
        clrscr();
    }
    auto scroll() -> void {
        uint32_t* vmem = (uint32_t*)LFB;
        for(int ly=0;ly<240;ly++) {
            for(int lx=0;lx<WIDTH*8;lx++) {
                if(ly<240-8) {
                    vmem[CALCXY(lx,ly)]=vmem[CALCXY(lx,ly+8)];
                } else {
                    vmem[CALCXY(lx,ly)]=0;
                }
            }
        }
/*        for(int x=0;x<WIDTH;x++) {
            for(int y=0;y<HEIGHT;y++) {
                for(int cx=0;cx<CHR_WIDTH;cx++) {
                    for(int cy=0; cy < CHR_HEIGHT; cy++) {
                        vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy)*BYTESPP]=vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy+8)*BYTESPP];
                        vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy)*BYTESPP+1]=vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy+8)*BYTESPP+1];
                        vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy)*BYTESPP+2]=vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy+8)*BYTESPP+2];
                    }
                }
            }
            int y=HEIGHT;
            for(int cx=0;cx<CHR_HEIGHT;cx++) {
                    for(int cy=0; cy < CHR_WIDTH; cy++) {
                        vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy)*BYTESPP]=0;
                        vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy)*BYTESPP+1]=0;
                        vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy)*BYTESPP+2]=0;
                    }
            }
        }*/
        y--;
    }
private:
    virtual auto putChar(int c) -> void{
        uint32_t* vmem = (uint32_t*)LFB;
        c&=0xFF;
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
            break;
        default:
            for(int cx=0;cx<CHR_WIDTH;cx++) {
                for(int cy=0;cy<CHR_HEIGHT;cy++) {
                    if(font[c][cy]&(1<<cx)) {
                        vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy)]=0xFFFFF00F;
                    }else {
                        vmem[CALCXY(x*CHR_WIDTH+cx,y*CHR_HEIGHT+cy)]=0;
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
        if(y>HEIGHT)
            scroll();
    }
};
}
}
void(*tbl[3])()={(void(*)())&getType,(void(*)())&size_of,(void(*)())&spawnAt};
table_type getTable(void*(*)(ModType)) {
    doCtors();
    return (void(**)())&tbl;
#ifdef ARM9
    return (void(**)())0x27FFFFE8;
#else
    return (void(**)())0x27FFFFF4;
#endif
}
auto getType() -> ModType {
    return ModType::output_text;
}
auto spawnAt(void* pos) -> bool {
    new(pos) MTGos::Screen;
    return true;
}
auto size_of() -> size_t {
    return sizeof(MTGos::Screen);
}
