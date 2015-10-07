#ifndef MMX_H
#define MMX_H

#ifdef __cplusplus
extern "C"
{
#endif

void hello(void);
long add2(long x, long y);
long add10_v1(long a, long b, long c, long d,
             long e, long f, long g, long h,
             long i, long j);

long add10_v2(long a, long b, long c, long d,
             long e, long f, long g, long h,
             long i, long j);

long min3_v1(long a, long b, long c);
long max3_v1(long a, long b, long c);

#ifdef __cplusplus
}
#endif

#endif // MMX_H
