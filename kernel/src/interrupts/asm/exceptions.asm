;; 2022 Ian Moffett <ian@kesscoin.com>

bits 64

extern log
extern clearScreen
extern canvas
extern hex2str
extern dump_exception_log

global divide_error
global debug_exception
global overflow_handler
global br_exception
global invalid_opcode_handler
global dev_not_avl
global double_fault
global invld_tss
global segment_not_present
global stack_segfault
global general_protection
global page_fault

%define S_PANIC 3
%define PANIC_BG_COLOR 0x00008B

%macro panic 1
    cli
    mov rdi, canvas
    mov rsi, PANIC_BG_COLOR
    call clearScreen

    mov rdi, vec_fired_msg
    mov rsi, S_PANIC
    call log

    ;; Convert error code to string.
    mov rdi, %1
    call hex2str

    ;; Write out error.
    mov rdi, rax
    mov rsi, -1
    call log

    mov rdi, double_newline
    mov rsi, -1
    call log

    ;; Write the panic message.
    mov rdi, panic_msg
    mov rsi, -1
    call log

    ;; Dump exception log
    mov rdi, rsp
    mov rsi, %1
    call dump_exception_log

    cli
    hlt
%endmacro

divide_error:
    panic 0x0

debug_exception:
    panic 0x1

overflow_handler:
    panic 0x4

br_exception:
    panic 0x5

invalid_opcode_handler:
    panic 0x6

dev_not_avl:
    panic 0x7

double_fault:
    panic 0x8

invld_tss:
    panic 0xA

segment_not_present:
    panic 0xB

stack_segfault:
    panic 0xC

general_protection:
    panic 0xD

page_fault:
    panic 0xE

panic_msg: db "Something went wrong! Below is some more technical information.", 0xA, 0xA, 0x0
vec_fired_msg: db "CAUGHT ", 0x0
double_newline: db 0xA, 0xA, 0x0
