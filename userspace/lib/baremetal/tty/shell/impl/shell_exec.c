#include <lib/baremetal/tty/shell/shell_exec.h>
#include <lib/baremetal/tty/vtty.h>

// 2022 Ian Moffett <ian@kesscoin.com>


void _shell_exec(struct Command command) {
    if (command.command == CT_ECHO) {
        libvtty_out(command.operand);    
    }
}
