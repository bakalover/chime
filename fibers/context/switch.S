.global setupcontext
.global switchcontext

setupcontext:
    movq %rsp, %r11 
    movq %rdi, %rsp 
    subq $64, %rsp
    andq $-16, %rsp
    addq $8, %rsp
    movq %rdx, 8(%rsp)
    pushq %rsi
    pushq $0 
    pushq $0 
    pushq $0
    pushq $0
    pushq $0
    pushq $0
    movq %rsp, %rax
    movq %r11, %rsp 
    retq

switchcontext:
    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12
    pushq %rbx
    pushq %rbp
    movq %rsp, (%rdi) 
    movq (%rsi), %rsp
    popq %rbp
    popq %rbx
    popq %r12
    popq %r13
    popq %r14
    popq %r15
    retq
