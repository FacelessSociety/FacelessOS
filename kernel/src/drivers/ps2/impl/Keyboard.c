#include <drivers/ps2/Keyboard.h>
#include <debug/log.h>
#include <arch/io/io.h>
#include <arch/io/legacypic.h>
#include <stddef.h>
#include <protection/r3kc.h>
#include <tty/vtty.h>

#define KEYB_IN_ISDIGIT(C) (C >= '0' && C <= '9')
#define KEYB_IN_ALPHA(C) (C >= 'a' && C <= 'z')


// 2022 Ian Moffett <ian@kesscoin.com>

static const char* const SC_ASCII = "\x00\x1B" "1234567890-=" "\x08"
"\x00" "qwertyuiop[]" "\x0D\x1D" "asdfghjkl;'`" "\x00" "\\"
"zxcvbnm,./" "\x00\x00\x00" " ";

__attribute__((interrupt)) void ps2_irq1_response(struct InterruptStackFrame*) {
    uint32_t scancode = inportb(0x60);
    char ch = SC_ASCII[scancode];

    if (scancode & 0x80) {
        PIC_sendEOI(1);
        return;
    }

    if ((vtty_initialized() && KEYB_IN_ISDIGIT(ch)) || KEYB_IN_ALPHA(ch) || ch == ' ') {
        vtty_feed(ch);          // Feed character into virtual TTY.
    } else if (ch == '\x08') {      // Backspace.
        vtty_pop();
    }


    PIC_sendEOI(1);
    inportb(0x60);
}
