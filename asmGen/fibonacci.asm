format ELF64 executable 3
entry start
include 'terminalLib/printf.asm'
include 'terminalLib/scanf.asm'
include 'terminalLib/sqrt.asm'
segment readable executable
start:
    call        main
    mov         rax, 1
    xor         rbx, rbx
    int         0x80
fib:
    push        rbp
    mov         rbp, rsp
    sub         rsp, 3 * 8
    mov         rax, 0
    mov         rdx, rax
    mov         rax, [rbp - -16]
    cmp         rax, rdx
    jne         .L6
    mov         rax, 0
    mov         rsp, rbp
    pop         rbp
    ret
.L6:
    mov         rax, 1
    mov         rdx, rax
    mov         rax, [rbp - -16]
    cmp         rax, rdx
    jne         .L8
    mov         rax, 1
    mov         rsp, rbp
    pop         rbp
    ret
.L8:
    push        rdx
    mov         rax, 1
    push        rax
    mov         rax, [rbp - -16]
    pop         rdx
    sub         rax, rdx
    push        rax
    call        fib
    pop         rdx
    mov         [rbp - 8], rax
    push        rdx
    mov         rax, 2
    push        rax
    mov         rax, [rbp - -16]
    pop         rdx
    sub         rax, rdx
    push        rax
    call        fib
    pop         rdx
    mov         [rbp - 16], rax
    mov         rax, [rbp - 16]
    push        rax
    mov         rax, [rbp - 8]
    pop         rdx
    add         rax, rdx
    mov         rsp, rbp
    pop         rbp
    ret
main:
    push        rbp
    mov         rbp, rsp
    sub         rsp, 2 * 8
    mov         rax, 0
    mov         [rbp - 8], rax
    mov         rax, 0
    mov         [rbp - 16], rax
    call        scanf
    mov         [rbp - 8], rax
    push        rdx
    mov         rax, [rbp - 8]
    push        rax
    call        fib
    pop         rdx
    mov         [rbp - 16], rax
    mov         rax, [rbp - 16]
    stdcall     printf, outInt,  rax
    mov         rax, 0
    mov         rsp, rbp
    pop         rbp
    ret
