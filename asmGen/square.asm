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
solveSquare:
    push        rbp
    mov         rbp, rsp
    sub         rsp, 4 * 8
    mov         rax, 0
    mov         rdx, rax
    mov         rax, [rbp - -16]
    cmp         rax, rdx
    jne         .L9
    mov         rax, 0
    mov         rdx, rax
    mov         rax, [rbp - -24]
    cmp         rax, rdx
    jne         .L11
    mov         rax, 0
    mov         rdx, rax
    mov         rax, [rbp - -32]
    cmp         rax, rdx
    jne         .L13
    mov         rax, -1
    stdcall     printf, outInt,  rax
    jmp         .L14
.L13:
    mov         rax, 0
    stdcall     printf, outInt,  rax
.L14:
    jmp         .L12
.L11:
    mov         rax, 1
    stdcall     printf, outInt,  rax
    mov         rax, [rbp - -24]
    push        rax
    mov         rax, [rbp - -32]
    push        rax
    mov         rax, -1
    pop         rdx
    imul        rax, rdx
    pop         rcx
    mov         rdx, rax
    shr         rdx, 32
    idiv        ecx
    cdqe
    stdcall     printf, outInt,  rax
.L12:
    jmp         .L10
.L9:
    mov         rax, [rbp - -32]
    push        rax
    mov         rax, [rbp - -16]
    push        rax
    mov         rax, 4
    pop         rdx
    imul        rax, rdx
    pop         rdx
    imul        rax, rdx
    push        rax
    mov         rax, [rbp - -24]
    push        rax
    mov         rax, [rbp - -24]
    pop         rdx
    imul        rax, rdx
    pop         rdx
    sub         rax, rdx
    mov         [rbp - 8], rax
    mov         rax, 0
    mov         rdx, rax
    mov         rax, [rbp - 8]
    cmp         rax, rdx
    jnb         .L15
    mov         rax, 0
    stdcall     printf, outInt,  rax
    jmp         .L16
.L15:
    mov         rax, 2
    stdcall     printf, outInt,  rax
    mov         rax, [rbp - -16]
    push        rax
    mov         rax, 2
    pop         rdx
    imul        rax, rdx
    push        rax
    mov         rax, [rbp - 8]
    push        rax
    call        sqrt
    add         rsp, 8
    push        rax
    mov         rax, [rbp - -24]
    push        rax
    mov         rax, -1
    pop         rdx
    imul        rax, rdx
    pop         rdx
    sub         rax, rdx
    pop         rcx
    mov         rdx, rax
    shr         rdx, 32
    idiv        ecx
    cdqe
    stdcall     printf, outInt,  rax
    mov         rax, [rbp - -16]
    push        rax
    mov         rax, 2
    pop         rdx
    imul        rax, rdx
    push        rax
    mov         rax, [rbp - 8]
    push        rax
    call        sqrt
    add         rsp, 8
    push        rax
    mov         rax, [rbp - -24]
    push        rax
    mov         rax, -1
    pop         rdx
    imul        rax, rdx
    pop         rdx
    add         rax, rdx
    pop         rcx
    mov         rdx, rax
    shr         rdx, 32
    idiv        ecx
    cdqe
    stdcall     printf, outInt,  rax
.L16:
.L10:
    mov         rax, 0
    mov         rsp, rbp
    pop         rbp
    ret
main:
    push        rbp
    mov         rbp, rsp
    sub         rsp, 3 * 8
    mov         rax, 0
    mov         [rbp - 8], rax
    mov         rax, 0
    mov         [rbp - 16], rax
    mov         rax, 0
    mov         [rbp - 24], rax
    call        scanf
    mov         [rbp - 8], rax
    call        scanf
    mov         [rbp - 16], rax
    call        scanf
    mov         [rbp - 24], rax
    mov         rax, [rbp - 24]
    push        rax
    mov         rax, [rbp - 16]
    push        rax
    mov         rax, [rbp - 8]
    push        rax
    call        solveSquare
    mov         rax, 0
    mov         rsp, rbp
    pop         rbp
    ret
