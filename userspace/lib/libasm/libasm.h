#ifndef LIBASM_H
#define LIBASM_H

// 2022 Ian Moffett <ian@kesscoin.com>

#include <stdint.h>

const volatile char* libasm_get_dateyear(void);
const volatile char* libasm_get_datemonth(void);
const volatile char* libasm_get_dateday(void);
void libasm_pcspkr_toggle(void);
void libasm_writech_xy(uint16_t x, uint16_t y, char c, uint32_t color);

#endif
