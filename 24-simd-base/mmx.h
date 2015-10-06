#ifndef MMX_H
#define MMX_H

#ifdef __cplusplus
extern "C"
{
#endif

void hello(void);
int add2(int x, int y);
int add10_v1(int a, int b, int c, int d,
             int e, int f, int g, int h,
             int i, int j);

int add10_v2(int a, int b, int c, int d,
             int e, int f, int g, int h,
             int i, int j);

#ifdef __cplusplus
}
#endif

#endif // MMX_H
