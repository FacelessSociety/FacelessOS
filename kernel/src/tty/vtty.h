#ifndef VTTY_H
#define VTTY_H

// 2022 Ian Moffett <ian@kesscoin.com>

#define VTTY_PROMPT "subgod@FacelessOS~> "

#include <stdint.h>


void vtty_init(void);
void vtty_feed(char c);
void vtty_pop(void);
uint8_t vtty_initialized(void);

#endif
