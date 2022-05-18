;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global libpiano_init

extern libvtty_pause
extern libvtty_resume
extern libvtty_scancode2ascii


%macro beep 1
    mov rax, 0xD
    mov r15, %1
    int 0x80
%endmacro


beep1:
    beep 523
    retq

beep2:
    beep 587
    retq

beep3:
    beep 659
    retq

beep4:
    beep 698
    retq

beep5:
    beep 784
    retq

beep6:
    beep 880
    retq


beep7:
    beep 988
    retq

beep8:
    beep 1047
    retq

beep9:
    beep 1175
    retq


quit:
    call libvtty_resume
    retq


slave_handler:
    call libvtty_scancode2ascii
    cmp rax, '1'
    je beep1
    cmp rax, '2'
    je beep2
    cmp rax, '3'
    je beep3
    cmp rax, '4'
    je beep4
    cmp rax, '5'
    je beep5
    cmp rax, '6'
    je beep6
    cmp rax, '7'
    je beep7
    cmp rax, '8'
    je beep8
    cmp rax, '9'
    je beep9
    cmp rax, 'q'
    je .quit
    retq

    .quit:
        call quit
        retq


libpiano_init:
    mov rdi, slave_handler          ;; First arg.
    call libvtty_pause
    retq
