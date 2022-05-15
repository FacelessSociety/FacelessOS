;; 2022 Ian Moffett <ian@kesscoin.com>
;; R3KC (Ring 3 Kernel Callback).
;; This file gives an interface
;; for a driver to call a routine
;; in ring3 without risking getting
;; stuck in ring 0.

;; THIS SHOULD ONLY BE USED FOR "HOOKS" WHICH HOOK ONTO AN INTERRUPT
;; ROUTINE AND THAT INTERRUPT ROUTINE NEEDS TO CALL A USERSPACE ROUTINE.

bits 64
global r3kc_exec_callback

;; <=== Args ===>
;; ARG0: CALLBACK_ADDRESS
;; OPARG1: OPTIONAL INFORMATION TO BRING TO USERSPACE ROUTINE (MAX SIZE: DWORD).
r3kc_exec_callback:
    mov [LOGICAL_CALLBACK_ADDRESS], rdi         ;; PRESERVE ARG0.
    mov [OPARG1], rsi                           ;; PRESERVE OPARG1.
    leave                                       ;; ENSURE THERE IS NO STACK FRAME FROM C.

    ;; THERE WILL BE AN IRET STACK-FRAME ON THE STACK.
    ;; THE RETURN ADDRESS IS ON THE TOP OF THE STACK,
    ;; WE WILL MODIFY THAT TO CHANGE THE INSTRUCTION
    ;; POINTER TO THE CALLBACK OFFSET.

    mov rdi, [LOGICAL_CALLBACK_ADDRESS]
    mov [rsp], rdi

    ;; NOTE: OPARG1 IS RETURNED IN RAX,
    ;; USERSPACE ROUTINE MUST CHECK RAX FOR THE ARGUMENT.

    mov rax, [OPARG1]

    ;; IN THE CASE THAT A ROUTINE WAS TO HANDLE
    ;; AND IRQ FROM PIC, WE WILL SEND AN END OF INTERRUPT
    ;; TO I/O PORT 0x20.

    mov ax, 0x20
    out 0x20, ax

    ;; PERFORM AN IRETQ.
    iretq

LOGICAL_CALLBACK_ADDRESS: dq 0
OPARG1: dd 0
