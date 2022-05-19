;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global libpiano_init

extern libvtty_pause
extern libvtty_resume
extern libvtty_scancode2ascii
extern rand_range

%define KEY_WIDTH 50
%define KEY_HEIGHT 150
%define KEY_Y 500
%define KEY_COLOR 0xFFFFFF
%define KEY_PRESSED_COLOR 0x2F2F2F
%define KEY1_X 100
%define KEY2_X 150
%define KEY2_X 250
%define KEY3_X 350
%define KEY4_X 450
%define KEY5_X 550
%define KEY6_X 650
%define KEY7_X 750
%define KEY8_X 850
%define KEY9_X 950



%macro beep 1
    mov rax, 0xD
    mov r15, %1
    int 0x80
%endmacro


;; Takes X pos.
%macro draw_key 2
    mov rax, 0x6
    mov r15, %1
    mov r14, KEY_Y
    mov r13, KEY_WIDTH
    mov r12, KEY_HEIGHT
    mov r11, %2
    int 0x80
%endmacro


draw_keys:
    draw_key KEY1_X, KEY_COLOR
    draw_key KEY2_X, KEY_COLOR
    draw_key KEY3_X, KEY_COLOR
    draw_key KEY4_X, KEY_COLOR
    draw_key KEY5_X, KEY_COLOR
    draw_key KEY6_X, KEY_COLOR
    draw_key KEY7_X, KEY_COLOR
    draw_key KEY8_X, KEY_COLOR
    draw_key KEY9_X, KEY_COLOR
    retq


beep1:
    draw_key KEY1_X, KEY_PRESSED_COLOR
    beep 523
    draw_key KEY1_X, KEY_COLOR
    retq

beep2:
    draw_key KEY2_X, KEY_PRESSED_COLOR
    beep 587
    draw_key KEY2_X, KEY_COLOR
    retq

beep3:
    draw_key KEY3_X, KEY_PRESSED_COLOR
    beep 659
    draw_key KEY3_X, KEY_COLOR
    retq

beep4:
    draw_key KEY4_X, KEY_PRESSED_COLOR
    beep 698
    draw_key KEY4_X, KEY_COLOR
    retq

beep5:
    draw_key KEY5_X, KEY_PRESSED_COLOR
    beep 784
    draw_key KEY5_X, KEY_COLOR
    retq

beep6:
    draw_key KEY6_X, KEY_PRESSED_COLOR
    beep 880
    draw_key KEY6_X, KEY_COLOR
    retq


beep7:
    draw_key KEY7_X, KEY_PRESSED_COLOR
    beep 988
    draw_key KEY7_X, KEY_COLOR
    retq

beep8:
    draw_key KEY8_X, KEY_PRESSED_COLOR
    beep 1047
    draw_key KEY8_X, KEY_COLOR
    retq

beep9:
    draw_key KEY9_X, KEY_PRESSED_COLOR
    beep 1175
    draw_key KEY9_X, KEY_COLOR
    retq


quit:
    call libvtty_resume
    retq


slave_handler:
    call libvtty_scancode2ascii

    .start:

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
    cmp rax, 'r'
    je .rand
    retq

    .quit:
        call quit
        retq

    .rand:
        mov rdi, 0
        mov rsi, 9
        call rand_range
        add rax, '0'
        jmp .start


libpiano_init:
    mov rdi, slave_handler          ;; First arg.
    call libvtty_pause

    call draw_keys

    retq
