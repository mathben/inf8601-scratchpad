# registers RDI, RSI, RDX, RCX, R8, and R9,

.global array_addss_iter, array_addss_vect

.text

# void array_add_scalar_iter(float *vector, float scalar, long length);
# rdi = vector
# xmm0 = scalar
# rsi  = length
array_addss_iter:
    cmp $0, %rsi
    jle 2f
1:
    movss %xmm0, %xmm1
    addss (%rdi), %xmm1
    movd %xmm1, %eax
    mov %eax, (%rdi)
    add $4, %rdi # next element
    dec %rsi
    jnz 1b
2:
    ret

# void array_add_scalar_vect(long *v, long off, long len);
array_addss_vect:
    ret
