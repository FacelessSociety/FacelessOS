;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global ring3_entry

extern libvtty_feed
extern libvtty_init


ring3_entry:
    call libvtty_init 
    jmp __loop


;; Just wait for keyboard interrupt.
__loop: 
    mov rax, 0x3            ;; SYS_INT_WAIT.
    int 0x80

    ;; If continued, an interrupt must have fired.
    mov rax, 0x1            ;; SYS_KEYBOARD_STATUS.
    int 0x80

    test r15, 1             ;; IRQ FIRED BIT.
    jz .end                 ;; If set, handle keypress.

    .handle_keypress:
        mov rax, 0x2        ;; SYS_KEYBOARD_SCANCODE.
        int 0x80            ;; SYSCALL (SCANCODE RETURNED IN R15).
        mov rdi, r15
        call libvtty_feed

    .end:

    jmp __loop
