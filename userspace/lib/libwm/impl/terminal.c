// 2022 Ian Moffett <ian@kesscoin.com>
#include <lib/libwm/terminal.h>
#include <lib/libwm/window.h>
#include <lib/libkeyboard/scancodes.h>
#include <lib/baremetal/tty/vtty.h>
#include <lib/libstring/string.h>
#include <lib/baremetal/tty/shell/interpreter.h>
#include <lib/baremetal/tty/shell/shell_exec.h>

#define TERMINAL_MAGIC 0xC01DB100D
#define INITIAL_Y 30
#define X_INC 8
#define Y_INC 15


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


void libwmterm_sync(void) {
    for (int i = 0; i < LIBWM_MAX_WINDOWS; ++i) {
        if (!(libwm_get_win(i).active)) terminals[i].magic = 0xDEADBEEF;            // Invalidate the terminal.
    }
}


static void reset_term(void);



static void prompt(void) {
    struct Window win = libwm_get_current_window();    

    // Check if line count is good, if not reset term.
    if (terminals[win.id].line_count > 15) { 
        reset_term();
    }

    terminals[win.id].y += Y_INC;
    terminals[win.id].x = 0;
    libwm_write_inside_win(VTTY_PROMPT, TERMINAL_TEXT_BG, terminals[win.id].x, terminals[win.id].y, win.id);
    terminals[win.id].x += 8 * _strlen(VTTY_PROMPT);
}


// Resets current terminal.
static void reset_term(void) {
    struct Window win = libwm_get_current_window();    
    terminals[win.id].line_count = 0;
    terminals[win.id].y = INITIAL_Y;
    terminals[win.id].buffer_idx = 0;
    _memzero(terminals[win.id].buffer, BUFFER_MAX_SZ);
    reset_win(win.id);
}


// Returns a copy of the current terminal.
struct Terminal libwmterm_get_current_term(void) {
    return terminals[libwm_get_current_window().id];
}


void libwmterm_write(const char* str) {
    struct Window win = libwm_get_current_window();
    libwm_write_inside_win(str, TERMINAL_TEXT_BG, terminals[win.id].x, terminals[win.id].y, win.id);
    terminals[win.id].x += X_INC*_strlen(str);          // Advance the position to after the string.
}


void libwmterm_feed(uint16_t scancode) {
    struct Window win = libwm_get_current_window();

    if (scancode & 0x80) return;      // Reject if it is released.
    if (terminals[win.id].buffer_idx >= BUFFER_MAX_SZ) return;      // Reject if buffer is full.

    // Sync all terminals.
    libwmterm_sync();

    // If magic doesn't match, set everything for this terminal up.
    if (terminals[win.id].magic != TERMINAL_MAGIC) {
        terminals[win.id].magic = TERMINAL_MAGIC;
        terminals[win.id].x = 0;
        terminals[win.id].y = INITIAL_Y;
        terminals[win.id].line_count = 0;
        terminals[win.id].buffer_idx = 0;
        _memzero(terminals[win.id].buffer, BUFFER_MAX_SZ);
        prompt();
    }


    switch (scancode) {
        case SC_ENTER:
            // Make a newline for the output of the command.
            terminals[win.id].y += Y_INC;
            terminals[win.id].x = 0;

            // Execute and clear the buffer.
            _shell_exec(shell_interp_process(terminals[win.id].buffer));  // Execute buffer.
            _memzero(terminals[win.id].buffer, BUFFER_MAX_SZ);            // Reset buffer.
            terminals[win.id].buffer_idx = 0;                             // Reset buffer idx.
        
            // Resend prompt.
            prompt();

            // Increment line count for active window.
            ++terminals[win.id].line_count;
            return; 
    }

    // Check if character is not backspace.
    if (SC_ASCII[scancode] == '\x08') {
        if (terminals[win.id].buffer_idx <= 0)
            // WILL NOT CONTINUE IF BUFFER IS EMPTY.
            return;

        terminals[win.id].x -= X_INC;           // Go back one.
        char terminated[2] = {terminals[win.id].buffer[terminals[win.id].buffer_idx - 1], 0x0};         // Same char will be written with same background as terminal to make it look gone.
        libwm_write_inside_win(terminated, TERMINAL_BG, terminals[win.id].x, terminals[win.id].y, win.id);     // Overwrite last char. 
        terminals[win.id].buffer[--terminals[win.id].buffer_idx] = '\0';                                        // Pop a char off the buffer.
        return;
    }

    char terminated[2] = {SC_ASCII[scancode], 0x0};
    libwm_write_inside_win(terminated, TERMINAL_TEXT_BG, terminals[win.id].x, terminals[win.id].y, win.id);
    terminals[win.id].x += X_INC;
    terminals[win.id].buffer[terminals[win.id].buffer_idx++] = SC_ASCII[scancode];
}
