// 2022 Ian Moffett <ian@kesscoin.com>
#include <lib/baremetal/tty/vtty.h>
#include <lib/libstring/string.h>
#include <stdint.h>


static const char* const SC_ASCII = "\x00\x1B" "1234567890-=" "\x08"
"\x00" "qwertyuiop[]" "\x0D\x1D" "asdfghjkl;'`" "\x00" "\\"
"zxcvbnm,./" "\x00\x00\x00" " ";


typedef enum {
    FLAG_INIT = (1 << 0)
} FLAG;


static uint8_t flags = 0x0;


static char libvtty_convert_scancode(uint16_t scancode) {
    return SC_ASCII[scancode];
}


void libvtty_feed(uint16_t scancode) {
    libvtty_writech(SC_ASCII[scancode]);
}


void libvtty_init(void) {
    if (flags & FLAG_INIT) return;
    flags |= FLAG_INIT;

    libvtty_writech('\n'); 

    for (size_t i = 0; i < _strlen(VTTY_PROMPT) - 1; ++i) {
        libvtty_writech(VTTY_PROMPT[i]);
    }
}
