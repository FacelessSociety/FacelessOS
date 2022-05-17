;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64
global libvtty_writech
global libvtty_pop

libvtty_writech:
    mov rax, 0x4            ;; SYS_BAREMETAL_WRITECH
    mov r15, rdi            ;; MOVING SCANCODE INTO R15.
    int 0x80                ;; SYSCALL.
    retq


libvtty_pop:
    mov rax, 0x5            ;; SYS_LOWER_CANVAS_X.
    int 0x80                ;; SYSCALL.

    mov rax, 0x8            ;; SYS_GET_CANVAS_Y.
    int 0x80                ;; SYSCALL.

    mov r14, r15            ;; MOVE Y INTO R14.

    mov rax, 0x7            ;; SYS_GET_CANVAS_X
    int 0x80                ;; SYSCALL.

    mov rax, 0x6            ;; SYS_LFB_DRAW_SQ
    mov r13, 1000           ;; WIDTH.
    mov r12, 20             ;; HEIGHT.
    mov r11, 0x000000       ;; COLOR.
    int 0x80                ;; SYSCALL.
    retq                    ;; RETURN.
