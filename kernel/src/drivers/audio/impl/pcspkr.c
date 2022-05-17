#include <drivers/audio/pcspkr.h>
#include <arch/io/io.h>


// 2022 Ian Moffett <ian@kesscoin.com>


void pcspkr_play(uint32_t hz) {
    uint32_t div = 1193180 / hz;
    outportb(0x43, 0xB6);                   // Command byte.
    outportb(0x42, (uint8_t)div);           // Low 8 bits.
    outportb(0x42, (uint8_t)(div >> 8));    // High 8 bits.
    
    uint8_t tmp = inportb(0x61);

    if (!(tmp & (1 << 0)) && !(tmp & (1 << 1))) {
        outportb(0x61, tmp | (1 << 0) | (1 << 1));
    }

}


void pcspkr_stop(void) {
    uint8_t tmp = inportb(0x61) & 0xFC;
    outportb(0x61, tmp);
}
