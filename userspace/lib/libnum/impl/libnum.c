#include <lib/libnum/libnum.h>


static unsigned short lfsr = 0xACE1u;
static unsigned bit;


unsigned rand(void) {
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    lfsr = (lfsr >> 1) | (bit << 15);
    return lfsr;
}

unsigned rand_range(uint8_t from, uint8_t to) {
    return rand() % (to - 1 + 1) + from;
}
