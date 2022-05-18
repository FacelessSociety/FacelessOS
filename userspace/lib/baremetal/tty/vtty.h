#ifndef LIB_VTTY_H
#define LIB_VTTY_H

#include <stdint.h>

#define VTTY_PROMPT "subgod@FacelessOS>  \0"


// 2022 Ian Moffett <ian@kesscoin.com>


void libvtty_feed(uint16_t scancode);
void libvtty_writech(char c);
void libvtty_init(void);
void libvtty_pop(void);
char libvtty_scancode2ascii(uint16_t scancode);
void libvtty_pause(void(*slave_handler)(uint16_t scancode));
void libvtty_resume(void);
void libvtty_out(const char* str);
void libvtty_out_oneline(const char* str);
uint64_t libvtty_get_x(void);

#endif
