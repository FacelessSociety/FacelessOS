// 2022 Ian Moffett <ian@kesscoin.com>
#include <lib/baremetal/tty/vtty.h>
#include <lib/baremetal/tty/shell/interpreter.h>
#include <lib/baremetal/tty/shell/shell_exec.h>
#include <lib/libstring/string.h>
#include <stdint.h>

#define BUFFER_MAX_SIZE 50
#define MAX_VTTY_LINES 15


static const char* const SC_ASCII = "\x00\x1B" "1234567890-=" "\x08"
"\x00" "qwertyuiop[]" "\x0D\x1D" "asdfghjkl;'`" "\x00" "\\"
"zxcvbnm,./" "\x00\x00\x00" " ";


typedef enum {
    FLAG_INIT = (1 << 0),
    FLAG_PAUSED = (1 << 1)
} FLAG;


static struct VTTYEnviron {
    uint8_t flags;
    uint16_t prompt_offset;
    size_t start_x;
    size_t cur_y;
    void(*slave_handler)(uint16_t);
} environ;


static char buffer[BUFFER_MAX_SIZE];


static char libvtty_convert_scancode(uint16_t scancode) {
    return SC_ASCII[scancode];
}


static void make_prompt(void) {
    // Make prompt.
    for (size_t i = 0; i < _strlen(VTTY_PROMPT) - 1; ++i) {
        libvtty_writech(VTTY_PROMPT[i]);
    }
}


void libvtty_out(const char* str) {
    libvtty_writech('\n');
    for (size_t i = 0; i < _strlen(str) - 1; ++i) {
        libvtty_writech(str[i]);
    }
}


void libvtty_out_oneline(const char* str) {
    for (size_t i = 0; i < _strlen(str) - 1; ++i) {
        libvtty_writech(str[i]);
    }
}


char libvtty_scancode2ascii(uint16_t scancode) {
    return SC_ASCII[scancode];
}

// Pauses the TTY and passes keyboard control to another program.
void libvtty_pause(void(*slave_handler)(uint16_t scancode)) { 
    __asm__ __volatile__("mov $0x9, %rax; int $0x80");          // SYS_CLEAR_SCREEN.
    environ.flags |= FLAG_PAUSED;
    environ.slave_handler = slave_handler;
}


void libvtty_resume(void) {
    __asm__ __volatile__("mov $0x9, %rax; int $0x80");          // SYS_CLEAR_SCREEN.
    environ.flags &= ~(FLAG_PAUSED);        // Simpily unset FLAG_PAUSED bit.
    environ.slave_handler = NULL;
    make_prompt();
}


void libvtty_feed(uint16_t scancode) {
    if (environ.flags & FLAG_PAUSED) {
       environ.slave_handler(scancode); 
       return;
    }

    if (scancode & 0x80) return;


    // If we hit backspace.
    if (SC_ASCII[scancode] == '\x08') {
        if (environ.prompt_offset <= 0) return;
        libvtty_pop();
        buffer[--environ.prompt_offset] = '\0';
        return;
    }

    switch (scancode) {
        case 28:                                    // Enter.
            if (environ.cur_y >= MAX_VTTY_LINES) {
                // Too many lines, clear up some space.
                environ.cur_y = 0;                                          // Reset y.
                __asm__ __volatile__("mov $0x9, %rax; int $0x80");          // SYS_CLEAR_SCREEN.
            }

            _shell_exec(shell_interp_process(buffer));
            environ.prompt_offset = 0;
            libvtty_writech('\n');
            _memzero(buffer, MAX_VTTY_LINES);       // Null out buffer. 
    
            // If the prompt is paused (because for example a command did it) we need to handler it.
            if (environ.flags & FLAG_PAUSED) return;
            ++environ.cur_y;
            make_prompt();
            return;
    }

    libvtty_writech(SC_ASCII[scancode]);
    buffer[environ.prompt_offset++] = SC_ASCII[scancode];
}


void libvtty_init(void) {
    environ.flags = 0;
    environ.flags |= FLAG_INIT;
    environ.start_x = libvtty_get_x();
    environ.cur_y = 0;
    environ.prompt_offset = 0; 
    make_prompt();
}
