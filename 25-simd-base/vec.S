# registers RDI, RSI, RDX, RCX, R8, and R9,

.global array_add_scalar_iter, array_add_scalar_vect

.text

# void array_add_scalar_iter(long *v, long off, long len);
# v = rdi
# off = rsi
# len = rdx
array_add_scalar_iter:
    mov %rdx, %rcx
    cmp $0, %rcx
    jle 2f
1:
    mov (%rdi), %rax
    add %rsi, %rax
    mov %rax,(%rdi)
    add $8, %rdi # next element
    dec %rcx
    jnz 1b
2:
    ret

# void array_add_scalar_vect(long *v, long off, long len);
array_add_scalar_vect:

    ret
