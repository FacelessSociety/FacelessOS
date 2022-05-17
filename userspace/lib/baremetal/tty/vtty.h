#ifndef LIB_VTTY_H
#define LIB_VTTY_H

#include <stdint.h>

#define VTTY_PROMPT "subgod@FacelessOS> \0"


// 2022 Ian Moffett <ian@kesscoin.com>


void libvtty_feed(uint16_t scancode);
void libvtty_writech(char c);
void libvtty_init(void);

#endif
