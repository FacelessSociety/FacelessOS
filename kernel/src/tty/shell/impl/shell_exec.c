#include <tty/shell/shell_exec.h>
#include <tty/vtty.h>
#include <debug/log.h>
#include <util/string.h>
#include <drivers/ps2/Controller.h>

// 2022 Ian Moffett <ian@kesscoin.com>


void shell_exec(struct Command command) {
    if (command.command == CT_ECHO) {
        for (size_t i = 0; i < strlen(command.operand); ++i) {
            vtty_feed(command.operand[i]);
        }
    } else if (command.command == CT_REBOOT) {
        ps2_send_reset();
    }
}
