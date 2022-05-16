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
extern PIC_sendEOI

;; <=== Args ===>
;; ARG0: CALLBACK_ADDRESS
;; OPARG1: OPTIONAL INFORMATION TO BRING TO USERSPACE ROUTINE (MAX SIZE: DWORD).
r3kc_exec_callback:
    leave

    ;; THERE WILL BE AN IRET STACK-FRAME ON THE STACK.
    ;; THE RETURN ADDRESS IS ON THE TOP OF THE STACK,
    ;; WE WILL MODIFY THAT TO CHANGE THE INSTRUCTION
    ;; POINTER TO THE CALLBACK OFFSET.
    mov [rsp], rdi

    ;; NOTE: OPARG1 IS RETURNED IN RAX,
    ;; USERSPACE ROUTINE MUST CHECK RAX FOR THE ARGUMENT.
    mov rax, rsi

    ;; SEND EOI (END OF INTERRUPT) TO PIC IN CASE
    ;; IN CASE THE ORIGIN OF INTERRUPT WAS AN IRQ.
    mov rdi, 1
    call PIC_sendEOI

    ;; PERFORM AN IRETQ.
    iretq

LOGICAL_CALLBACK_ADDRESS: dq 0
OPARG1: dd 0
