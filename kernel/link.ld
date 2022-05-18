OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS 
{
    .text : ALIGN(0x1000)
    {
        EXCLUDE_FILE(*.kernel.o)
        *(.text)
    }

    .data : ALIGN(0x1000) 
    {
        EXCLUDE_FILE(*.kernel.o)
        *(.data)
    }

    .rodata : ALIGN(0x1000) 
    {
        EXCLUDE_FILE(*.kernel.o)
        *(.rodata)
    }

    .bss : ALIGN(0x1000) 
    {
        EXCLUDE_FILE(*.kernel.o)
        *(COMMON)
        *(.bss)
    }

    .kernel_reserved : ALIGN(0x1000)
    {
        *.kernel.o
    }
}
