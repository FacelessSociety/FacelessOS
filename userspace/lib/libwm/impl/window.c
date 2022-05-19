// 2022 Ian Moffett <ian@kesscoin.com>
#include <lib/libwm/window.h>
#include <lib/libgfx/libgfx.h>


static struct Window windows[LIBWM_MAX_WINDOWS];
__attribute__((section(".data"))) static uint8_t cur_id = 0;


void libwm_create_window(void) {
    struct Window newwin;
    if (cur_id >= LIBWM_MAX_WINDOWS) return;

    newwin.id = cur_id;
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
    libgfx_draw_square(newwin.xpos, newwin.ypos, newwin.width, newwin.height, 0x808080);
}


void libwm_pop_window(void) {
    --cur_id;
    libgfx_draw_square(windows[cur_id].xpos, windows[cur_id].ypos, windows[cur_id].width, windows[cur_id].height, 0x000000);
}
