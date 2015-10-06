#
# x86_64 calling convention:
# https://en.wikipedia.org/wiki/X86_calling_conventions
#
# The first six integer or pointer arguments are passed in
# registers RDI, RSI, RDX, RCX, R8, and R9,
# while XMM0-7 are used for floating point arguments.
#

.global hello, add2, add10_v1, add10_v2

.text

# the classical hello world test
msg:
        .ascii  "hello\n"
hello:
    mov     $1, %rax
    mov     $1, %rdi
    mov     $msg, %rsi
    mov     $6, %rdx
    syscall
    ret

# add two integers, do not modify arguments
add2:
    mov %rdi, %rax
    add %rsi, %rax
    ret

add10_v1:
    # arguments in registers
    mov %rdi, %rax
    add %rsi, %rax
    add %rdx, %rax
    add %rcx, %rax
    add %r8, %rax
    add %r9, %rax

    # arguments on the stack
    add 0x08(%rsp), %rax
    add 0x10(%rsp), %rax
    add 0x18(%rsp), %rax
    add 0x20(%rsp), %rax

    ret

add10_v2:
    # arguments in registers
    mov %rdi, %rax
    add %rsi, %rax
    add %rdx, %rax
    add %rcx, %rax
    add %r8, %rax
    add %r9, %rax

    # aguments on the stack
    # using base pointer instead of stack pointer
    push %rbp
    mov %rsp, %rbp
    add 0x10(%rbp), %rax
    add 0x18(%rbp), %rax
    add 0x20(%rbp), %rax
    add 0x28(%rbp), %rax
    pop %rbp

    ret
