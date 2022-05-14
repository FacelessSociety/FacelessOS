;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global gdt_load
global GDT_TSS

section .kess_kernel_reserved
GDT:
    .null: equ $ - GDT
        dd 0x0
        dd 0x0
    .code: equ $ - GDT
        dw 0                    ;; Limit low.
        dw 0                    ;; Base low.
        db 0                    ;; Base middle.
        db 0b10011010           ;; Access.
        db 0b10101111           ;; Granularity.
        db 0                    ;; Base high.
    .data: equ $ - GDT
        dw 0                    ;; Limit low.
        dw 0                    ;; Base low.
        db 0                    ;; Base middle.
        db 0b10010010           ;; Access.
        db 0b00000000           ;; Granularity.
        db 0                    ;; Base high.
    .user_code: equ $ - GDT
        dw 0                    ;; Limit low.
        dw 0                    ;; Base low.
        db 0                    ;; Base middle.
        db 0b11111010           ;; Access.
        db 0b10101111           ;; Granularity.
        db 0                    ;; Base high.
    .user_data: equ $ - GDT
        dw 0                    ;; Limit low.
        dw 0                    ;; Base low.
        db 0                    ;; Base middle.
        db 0b11110010           ;; Access.
        db 0b00000000           ;; Granularity.
        db 0                    ;; Base high.
    TSS:
        dq 0
        dq 0
    Pointer:
        dw $ - GDT - 1          ;; Limit.
        dq GDT                  ;; Base.


GDT_TSS: dq 0

gdt_load:
    lgdt [Pointer]              ;; Load GDT via pointer.
    mov ax, 0x10                ;; Set AX to GDT data selector.
    mov ds, ax                  ;; Set DS to data selector.
    mov es, ax                  ;; Set ES to data selector.
    mov fs, ax                  ;; Set FS to data selector.
    mov gs, ax                  ;; Set GS to data selector.
    mov ss, ax                  ;; Set SS to data selector.
    mov rax, 0x8                ;; Set RAX to code selector.    
    pop rdi                     ;; Pop return address into RDI.
    push rax                    ;; Push code selector of descriptor to change CS to.
    push rdi                    ;; Push return address.
    retfq                       ;; Far return.
