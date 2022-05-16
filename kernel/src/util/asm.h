#ifndef ASM_H
#define ASM_H

// 2022 Ian Moffett <ian@kesscoin.com>

#define CLI __asm__ __volatile__("cli")
#define STI __asm__ __volatile__("sti")
#define HLT __asm__ __volatile__("hlt")
#define SECTION_DATA __attribute__((section(".data")))

#endif
