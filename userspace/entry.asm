;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global ring3_entry

keyboard_response:
    xor rax, rax
    int 0x80
    jmp __loop

ring3_entry: 
    mov rax, 0x1
    mov r15, keyboard_response
    int 0x80
    jmp __loop

__loop: jmp $
