#ifndef VEC_H
#define VEC_H

#ifdef __cplusplus
extern "C"
{
#endif

void array_addss_iter(float *vector, float scalar, long length);
void array_addss_vect(float *vector, float scalar, long length);

#ifdef __cplusplus
}
#endif

#endif // VEC_H
