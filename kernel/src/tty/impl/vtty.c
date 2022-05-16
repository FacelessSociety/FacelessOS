#include <tty/vtty.h>
#include <tty/shell/interpreter.h>
#include <tty/shell/shell_exec.h>
#include <debug/log.h>
#include <stdint.h>
#include <util/asm.h>
#include <util/string.h>
#include <util/mem.h>
#include <drivers/video/FrameBuffer.h>

// 2022 Ian Moffett <ian@kesscoin.com>

#define MAX_BUFFER_SZ 30


static uint8_t initialized = 0;
SECTION_DATA static char buffer[MAX_BUFFER_SZ];
static uint16_t buffer_idx = 0;
extern canvas_t canvas;


void vtty_init(void) {
    initialized = 1;
    buffer_idx = 0;
    log("\n%s", -1, VTTY_PROMPT);
}


void vtty_feed(char c) {
    if (buffer_idx >= MAX_BUFFER_SZ - 1) return;

    buffer[buffer_idx++] = c;
    char terminated_ch[2] = {c, 0x0};
    log("%s", -1, terminated_ch);
}


void vtty_submit_command(void) {
    buffer[buffer_idx] = '\0';
    struct Command cmd = shell_interp_process(buffer);  
    
    for (int i = 0; i < MAX_BUFFER_SZ; ++i) {
        vtty_pop();
    }

    shell_exec(cmd);
}


void vtty_pop(void) {
    if (buffer_idx == 0) return;
    canvas.x -= 8;
    draw_square(canvas, canvas.x, canvas.y, 50, 50, 0x000000);
    buffer[--buffer_idx] = '\0';
}


uint8_t vtty_initialized(void) {
    return initialized;
}
