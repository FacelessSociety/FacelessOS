// 2022 Ian Moffett <ian@kesscoin.com>
#include <lib/baremetal/tty/vtty.h>
#include <lib/baremetal/tty/shell/interpreter.h>
#include <lib/baremetal/tty/shell/shell_exec.h>
#include <lib/libstring/string.h>
#include <stdint.h>

#define BUFFER_MAX_SIZE 50


static const char* const SC_ASCII = "\x00\x1B" "1234567890-=" "\x08"
"\x00" "qwertyuiop[]" "\x0D\x1D" "asdfghjkl;'`" "\x00" "\\"
"zxcvbnm,./" "\x00\x00\x00" " ";


typedef enum {
    FLAG_INIT = (1 << 0)
} FLAG;


static struct VTTYEnviron {
    uint8_t flags;
    uint16_t prompt_offset;
    size_t start_x;
} environ;


static char buffer[BUFFER_MAX_SIZE];


static char libvtty_convert_scancode(uint16_t scancode) {
    return SC_ASCII[scancode];
}


void libvtty_out(const char* str) {
    libvtty_writech('\n');
    for (size_t i = 0; i < _strlen(str) - 1; ++i) {
        libvtty_writech(str[i]);
    }
}


void libvtty_feed(uint16_t scancode) {
    if (SC_ASCII[scancode] == '\x08') {
        if (environ.prompt_offset <= 0) return;
        libvtty_pop();
        buffer[--environ.prompt_offset] = '\0';
        return;
    } else if (environ.prompt_offset >= BUFFER_MAX_SIZE) {
        return;
    } else if (scancode == 28) {        // Enter.
        _shell_exec(shell_interp_process(buffer));      // Execute buffer.
        environ.prompt_offset = 0;
        _memzero(buffer, BUFFER_MAX_SIZE);
        libvtty_writech('\n');          // Make space for new prompt.
    
        // Make prompt.
        for (size_t i = 0; i < _strlen(VTTY_PROMPT) - 1; ++i) {
            libvtty_writech(VTTY_PROMPT[i]);
        }

        return;

    }

    libvtty_writech(SC_ASCII[scancode]);
    buffer[environ.prompt_offset++] = SC_ASCII[scancode];
}


void libvtty_init(void) {
    environ.flags = 0;
    environ.prompt_offset = 0;
    environ.flags |= FLAG_INIT;
    environ.start_x = libvtty_get_x();

    for (size_t i = 0; i < _strlen(VTTY_PROMPT) - 1; ++i) {
        libvtty_writech(VTTY_PROMPT[i]);
    }
}
