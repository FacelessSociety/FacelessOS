#ifndef LIBWM_TERMINAL_H
#define LIBWM_TERMINAL_H

#include <stdint.h>

#define TERMINAL_TEXT_BG 0xFFFFFF
#define BUFFER_MAX_SZ 30

// 2022 Ian Moffett <ian@kesscoin.com>

struct Terminal {
    uint16_t x, y;
    uint64_t magic;
    uint16_t line_count;
    uint16_t buffer_idx;
    char buffer[BUFFER_MAX_SZ];
};


void libwmterm_feed(uint16_t scancode);

/*
 *  This is like feed but instead of storing stuff into a buffer
 *  it just writes it onto the terminal window.
 *
 */

void libwmterm_write(const char* str);
void libwmterm_sync(void);
struct Terminal libwmterm_get_current_term(void);

#endif
