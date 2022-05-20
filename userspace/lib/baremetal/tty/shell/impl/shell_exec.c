#include <lib/baremetal/tty/shell/shell_exec.h>
#include <lib/baremetal/tty/vtty.h>
#include <lib/libpiano/libpiano.h>
#include <lib/libasm/libasm.h>
#include <lib/libwm/wm.h>
#include <lib/libwm/window.h>
#include <lib/libwm/terminal.h>
#include <lib/libstring/string.h>

// 2022 Ian Moffett <ian@kesscoin.com>

__attribute__((section(".data"))) static uint8_t gui_mode = 0;


static void out(const char* str, uint8_t oneline) {
    struct Terminal current = libwmterm_get_current_term();
    struct Window current_win = libwm_get_current_window();

    if (!(gui_mode) && !(oneline)) {
        libvtty_out(str);
    } else if (current_win.active && current_win.type == BWT_TERMINAL) {
        libwmterm_write(str);
    } else if (oneline) {
        libvtty_out_oneline(str);
    }
}


void _shell_exec(struct Command command) {
    switch (command.command) {
        case CT_ECHO:
            out(command.operand, 0);
            break;
        case CT_DATE:
            // Make a newline with libvtty_out() but the rest we don't need newlines.
            out((char*)libasm_get_datemonth(), 0);
            out("/", 1);
            out((char*)libasm_get_dateday(), 0);
            out("/", 1);
            out((char*)libasm_get_dateyear(), 0);
            break;
        case CT_PCSPKR_TOGGLE:
            libasm_pcspkr_toggle();
            break;
        case CT_PIANO:
            if (!(gui_mode)) {
                libpiano_init();
            }

            break;
        case CT_DMPPCI:
            if (!(gui_mode)) {
                __asm__ __volatile__("mov $0xE, %rax; int $0x80");      // SYS_LSPCI.
            }

            break;
        case CT_REBOOT:
            __asm__ __volatile__("mov $0xF, %rax; int $0x80");
            break;
            libwm_create_window(BWT_TERMINAL);
        case CT_WM:
            if (!(gui_mode)) {
                libwm_init();
                gui_mode = 1;
            }
            break;
    }
}
