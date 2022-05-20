#include <lib/libwm/wm.h>
#include <lib/libwm/window.h>
#include <lib/libwm/terminal.h>
#include <lib/baremetal/tty/vtty.h>
#include <stdint.h>

// 2022 Ian Moffett <ian@kesscoin.com>


static const char* const SC_ASCII = "\x00\x1B" "1234567890-=" "\x08"
"\x00" "qwertyuiop[]" "\x0D\x1D" "asdfghjkl;'`" "\x00" "\\"
"zxcvbnm,./" "\x00\x00\x00" " ";


static void handler(uint16_t scancode) {
    static uint8_t ctrl_pressed = 0;

    if (ctrl_pressed && scancode & 0x80)
        ctrl_pressed = 0;

    if (ctrl_pressed) {
        if (scancode == 1) {            // Escape.
            libwm_destroyall();
            libvtty_resume();
            return;
        }

        switch (SC_ASCII[scancode]) {
            case 't':
                libwm_create_window(BWT_TERMINAL);
                return;
            case 'q':
                libwm_pop_window();
                return;
        } 
    }

    struct Window current = libwm_get_current_window();

    // Interact with current window if active.
    if (current.active) {
        switch (current.type) {
            case BWT_TERMINAL:
                libwmterm_feed(scancode);
                break;
        }
    }

    if (scancode == 29 && !(scancode & 0x80)) 
        ctrl_pressed = 1;
}


void libwm_init(void) {
    libvtty_pause(handler);         // Pass control to handler().
}
