#include <tty/vtty.h>
#include <debug/log.h>
#include <stdint.h>
#include <util/asm.h>
#include <drivers/video/FrameBuffer.h>

#define MAX_BUFFER_SZ 30


static uint8_t initialized = 0;
SECTION_DATA static char buffer[MAX_BUFFER_SZ];
static uint16_t buffer_idx = 0;


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


void vtty_pop(void) {
    if (buffer_idx == 0) return;


    extern canvas_t canvas;
    canvas.x -= 8;
    draw_square(canvas, canvas.x, canvas.y, 15, 15, 0x000000);
    buffer[--buffer_idx] = '\0';
}


uint8_t vtty_initialized(void) {
    return initialized;
}
