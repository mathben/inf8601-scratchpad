#ifndef VEC_H
#define VEC_H

#ifdef __cplusplus
extern "C"
{
#endif

void array_add_scalar_iter(long *v, long off, long len);
void array_add_scalar_vect(long *v, long off, long len);

#ifdef __cplusplus
}
#endif

#endif // VEC_H
