;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global ring3_entry

ring3_entry: 
    jmp __loop

__loop: jmp $
