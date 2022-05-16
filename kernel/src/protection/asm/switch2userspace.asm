;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global switch_to_userspace

extern ring3_entry

switch_to_userspace:
    mov rax, 0x20 | 0x3         ;; user Code selector (bottom bits are for RPL).
    mov ds, rax
    mov es, rax
    mov fs, rax
    mov gs, rax

    ;; We will trick the processor into
    ;; thinking we are already in usermode
    ;; by using IRET.
    ;; IRET expects a stack frame 
    ;; that we will set up for it.
    ;; STACK BEFORE IRET:

    ;; |-----|
    ;; | SS  | 0x1
    ;; |-----| 
    ;; | RSP | 0x4
    ;; |-----|
    ;; |RFLAG| 0x8
    ;; |  S  | 
    ;; |-----|
    ;; |  CS | 0xC
    ;; |-----|
    ;; | RIP | 0x12
    ;; |     |
    ;; |-----|


    push rax                ;; SS.

    mov rax, rsp        
    push rax                ;; RSP.

    pushf                   ;; RFLAGS.

    mov rax, 0x18 | 0x3     ;; User code selector.
    push rax
    mov rax, ring3_entry    
    push rax                ;; RIP.
    iretq

