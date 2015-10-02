        .global hello
        .text
hello:
        mov     $1, %rax
        mov     $1, %rdi
        mov     $msg, %rsi
        mov     $13, %rdx
        syscall
        ret

msg:
        .ascii  "hello\n"
