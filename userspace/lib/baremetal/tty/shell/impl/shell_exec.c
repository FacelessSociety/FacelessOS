#include <lib/baremetal/tty/shell/shell_exec.h>
#include <lib/baremetal/tty/vtty.h>
#include <lib/libasm/libasm.h>

// 2022 Ian Moffett <ian@kesscoin.com>

void _shell_exec(struct Command command) {
    switch (command.command) {
        case CT_ECHO:
            libvtty_out(command.operand);
            break;
        case CT_DATE:
            // Make a newline with libvtty_out() but the rest we don't need newlines.
            libvtty_out((char*)libasm_get_datemonth());
            libvtty_out_oneline("/");
            libvtty_out_oneline((char*)libasm_get_dateday());
            libvtty_out_oneline("/");
            libvtty_out_oneline((char*)libasm_get_dateyear());
            break;
        case CT_PCSPKR_TOGGLE:
            libasm_pcspkr_toggle();
            break;
    }
}
