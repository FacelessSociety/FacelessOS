#include <tty/shell/shell_exec.h>
#include <tty/vtty.h>
#include <debug/log.h>
#include <util/string.h>
#include <util/baremetal_style.h>
#include <drivers/ps2/Controller.h>
#include <drivers/rtc/rtc.h>
#include <drivers/video/FrameBuffer.h>

// 2022 Ian Moffett <ian@kesscoin.com>

extern canvas_t canvas;


void shell_exec(struct Command command) {
    if (command.command == CT_ECHO) {
        vtty_feed_bulk((char*)command.operand);
    } else if (command.command == CT_REBOOT) {
        ps2_send_reset();
    } else if (command.command == CT_DATE) {
        struct RTCDateTime datetime = rtc_read_date();

        // Write out datetime.
        
        vtty_feed_bulk(dec2str(datetime.month));
        vtty_feed('/');
        vtty_feed_bulk(dec2str(datetime.day));
        vtty_feed('/');
        vtty_feed_bulk(dec2str(datetime.year));
        vtty_feed(' ');
    } else if (command.command == CT_CLEAR) {
        clearScreen(&canvas, BG_COLOR);
        log(VTTY_PROMPT, -1);
    }
}
