#include <lib/libstring/string.h>
#include <lib/libasm/libasm.h>


size_t _strlen(const char* const STR) {
    size_t n = 0;
    while (STR[n++]);
    return n;
}

uint8_t _strncmp(char* s1, char* s2, uint32_t n) {
    uint32_t n_cnt = 0;

    while (s1 && s2 && n_cnt < n) {
        if (*s1++ != *s2++) {
            return 0;
        }

        ++n_cnt;
    }

    return 1;
}


void _memzero(void* ptr, size_t n) {
    char* ptr_char = (char*)ptr;

    for (size_t i = 0; i < n; ++i) {
        ptr_char[i] = 0;
    }
}



void _write_str(const char* str, uint16_t x, uint16_t y, uint32_t color) {
    for (int i = 0; i < _strlen(str); ++i) {
    }
}

void libstring_writestr_xy(uint16_t x, uint16_t y, const char* str, uint32_t color) {
    for (size_t i = 0; i < _strlen(str); ++i) {
        libasm_writech_xy(x, y, str[i], color);
        x += 8;
    }
}
