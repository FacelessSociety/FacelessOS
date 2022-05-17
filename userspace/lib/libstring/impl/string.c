#include <lib/libstring/string.h>


size_t _strlen(const char* const STR) {
    size_t n = 0;
    while (STR[n++]);
    return n;
}
