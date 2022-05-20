#include <lib/libwm/terminal.h>
#include <lib/libwm/window.h>

#define TERMINAL_TEXT_BG 0xFFFFFF
#define TERMINAL_MAGIC 0xC01DB100D

struct Terminal {
    uint16_t x, y;
    uint64_t magic;
};

__attribute__((section(".data"))) static struct Terminal terminals[LIBWM_MAX_WINDOWS];


static const char* const SC_ASCII = "\x00\x1B" "1234567890-=" "\x08"
"\x00" "qwertyuiop[]" "\x0D\x1D" "asdfghjkl;'`" "\x00" "\\"
"zxcvbnm,./" "\x00\x00\x00" " ";

/*
 * Does a check on all terminals
 * if the window that maps to that terminal
 * is inactive then we should invalidate that terminal
 * by changing it's magic number to 0xDEADBEEF.
 */

static void sync(void) {
    for (int i = 0; i < LIBWM_MAX_WINDOWS; ++i) {
        if (!(libwm_get_win(i).active)) terminals[i].magic = 0xDEADBEEF;            // Invalidate the terminal.
    }
}



void libwmterm_feed(uint16_t scancode) {
    if (scancode & 0x80) return;                // Reject if it is released.

    // Sync all terminals.
    sync();

    struct Window win = libwm_get_current_window();

    if (terminals[win.id].magic != TERMINAL_MAGIC) {
        terminals[win.id].magic = TERMINAL_MAGIC;
        terminals[win.id].x = 0;
        terminals[win.id].y = 30;
    }


    char terminated[2] = {SC_ASCII[scancode], 0x0};
    libwm_write_inside_win(terminated, TERMINAL_TEXT_BG, terminals[win.id].x, terminals[win.id].y, win.id);
    terminals[win.id].x += 8;
}
