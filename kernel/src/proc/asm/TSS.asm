;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global load_task_register

load_task_register:
    str bx
    mov bx, 0x28
    ltr bx
    retq
