#ifndef LIB_STRING_H
#define LIB_STRING_H

// 2022 Ian Moffett <ian@kesscoin.com>

#include <stddef.h>
#include <stdint.h>

size_t _strlen(const char* const STR);
uint8_t _strncmp(char* s1, char* s2, uint32_t n);
void _memzero(void* ptr, size_t n);


#endif