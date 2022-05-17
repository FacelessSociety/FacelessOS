#ifndef LIBASM_H
#define LIBASM_H

// 2022 Ian Moffett <ian@kesscoin.com>

const volatile char* libasm_get_dateyear(void);
const volatile char* libasm_get_datemonth(void);
const volatile char* libasm_get_dateday(void);
void libasm_pcspkr_toggle(void);

#endif
