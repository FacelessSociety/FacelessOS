;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64
global libvtty_writech
global libvtty_pop
global libvtty_get_x

libvtty_writech:
    mov rax, 0x4            ;; SYS_BAREMETAL_WRITECH
    mov r15, rdi            ;; MOVING SCANCODE INTO R15.
    int 0x80                ;; SYSCALL.
    retq


;; Pops a character from the prompt.
;; NOTE: If a buffer is being used,
;; the buffer will need to be popped
;; manually.
libvtty_pop:
    mov rax, 0x5            ;; SYS_LOWER_CANVAS_X.
    int 0x80                ;; SYSCALL.

    mov rax, 0x8            ;; SYS_GET_CANVAS_Y.
    int 0x80                ;; SYSCALL.

    mov r14, r15            ;; MOVE Y INTO R14.

    mov rax, 0x7            ;; SYS_GET_CANVAS_X
    int 0x80                ;; SYSCALL.

    mov rax, 0x6            ;; SYS_LFB_DRAW_SQ
    mov r13, 20             ;; WIDTH.
    mov r12, 20             ;; HEIGHT.
    mov r11, 0x000000       ;; COLOR.
    int 0x80                ;; SYSCALL.
    retq                    ;; RETURN.

libvtty_get_x:
    mov rax, 0x5            ;; SYS_GET_CANVAS_X
    int 0x80                ;; SYSCALL.
    mov rax, r15            ;; MOVE CANVAS_X INTO RAX.
    retq
