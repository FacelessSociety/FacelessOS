// 2022 Ian Moffett <ian@kesscoin.com>
#include <lib/libwm/window.h>
#include <lib/libgfx/libgfx.h>
#include <lib/libstring/string.h>


static struct Window windows[LIBWM_MAX_WINDOWS];
__attribute__((section(".data"))) static uint8_t cur_id = 0;

static void draw_topbar(uint8_t id, BUILTIN_WIN_TYPE type, uint8_t active) {
    if (id >= LIBWM_MAX_WINDOWS || id < 0) return;

    const uint8_t BAR_HEIGHT = 20;

    if (active)
        libgfx_draw_square(windows[id].xpos, windows[id].ypos, windows[id].width, BAR_HEIGHT, 0xADD8E6);
    else
        libgfx_draw_square(windows[id].xpos, windows[id].ypos, windows[id].width, BAR_HEIGHT, 0x232B2B);
    

    const uint8_t ICON_WIDTH = 30;
    const uint8_t ICON_HEIGHT = BAR_HEIGHT;
    const uint32_t ICON_BG = 0x1D2021;

    switch (type) {
        case BWT_TERMINAL:
            libgfx_draw_square(windows[id].xpos, windows[id].ypos, ICON_WIDTH, ICON_HEIGHT, ICON_BG);
            libstring_writestr_xy(windows[id].xpos, windows[id].ypos, ">_", 0xFFEA00);
            break;
    }
}


void libwm_write_inside_win(const char* str, uint32_t color, uint16_t x, uint16_t y, uint8_t winid) {
    if (winid > LIBWM_MAX_WINDOWS) return;
    struct Window target = windows[winid];

    if ((target.width - 1) - x < 0 || target.ypos + y > (target.height - 1) || y < 15) return;
    libstring_writestr_xy(target.xpos + x, target.ypos + y, str, color);
}


void libwm_create_window(BUILTIN_WIN_TYPE type) {
    struct Window newwin;
    if (cur_id >= LIBWM_MAX_WINDOWS) return; 

    newwin.id = cur_id;
    newwin.type = type;
    newwin.active = 1;
    if (cur_id == 0) {
        newwin.width = LIBWM_DEFAULT_WIDTH;
        newwin.height = LIMWM_DEFAULT_HEIGHT;
        newwin.xpos = LIBWM_DEFAULT_X;
        newwin.ypos = LIBWM_DEFAULT_Y;
    } else {
        newwin.width = windows[cur_id - 1].width / 2;
        newwin.height = windows[cur_id - 1].height;
        newwin.ypos = windows[cur_id - 1].ypos;
        newwin.xpos = (windows[cur_id - 1].xpos + windows[cur_id - 1].width) + 2;
    }

    windows[cur_id++] = newwin;
    libgfx_draw_square(newwin.xpos, newwin.ypos, newwin.width, newwin.height, LIBWM_DEFAULT_WIN_COLOR);
    draw_topbar(cur_id - 1, type, 1);
    draw_topbar(cur_id - 2, type, 0);
}


void libwm_pop_window(void) {
    if (cur_id == 0) return;

    --cur_id;
    libgfx_draw_square(windows[cur_id].xpos, windows[cur_id].ypos, windows[cur_id].width, windows[cur_id].height, 0x000000);
    windows[cur_id].active = 0;
    draw_topbar(cur_id - 1, windows[cur_id - 1].type, 1);
}


void libwm_destroyall(void) {
    for (int i = 0; i < LIBWM_MAX_WINDOWS; ++i) {
        libwm_pop_window();
    }
}


struct Window libwm_get_current_window(void) {
    return windows[cur_id - 1];
}



/*
 *  Returns a window with xpos & ypos as 999 and active as 0
 *  if invalid id.
 */

struct Window libwm_get_win(uint8_t id) {
    if (id >= LIBWM_MAX_WINDOWS) {
        struct Window invalid = {
            .xpos = 999,
            .ypos = 999,
            .active = 0
        };

        return invalid;
    }
    return windows[id];
}
