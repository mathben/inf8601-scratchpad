#include <QDebug>
#include <QString>
#include <cstdio>
#include <cassert>

#include "ops.h"
#include "vec.h"
#include <xmmintrin.h>

__attribute__((noinline))
float fadd_cpp(float x, float y)
{
    return x + y;
}


__attribute__((noinline))
void array_addss_cpp(float *vector, float scalar, int length)
{
    for (int i = 0; i < length; i++) {
        vector[i] = vector[i] + scalar;
    }
}


void assembly_ops()
{
    hello();

    std::printf("%lX\n", add2(1, 2));

    long cafebabe1 = add10_v1(0xCAFE0000, 0, 0, 0, 0, 0, 0, 0, 0, 0x0000BABE);
    long cafebabe2 = add10_v2(0xCAFE0000, 0, 0, 0, 0, 0, 0, 0, 0, 0x0000BABE);
    std::printf("%lX\n", cafebabe1);
    std::printf("%lX\n", cafebabe2);

    std::printf("%ld\n", min3_v1(3, 10, -12));
    std::printf("%ld\n", max3_v1(3, 10, -12));

    std::printf("%ld\n", eq2(1, 2));
    std::printf("%ld\n", eq2(2, 2));

    std::printf("%f\n", fadd_cpp(1.1, 1.1));
    std::printf("%f\n", fadd2(1.1, 1.1));
}

void reset(QVector<float> &vec)
{
    for (int i = 0; i < vec.size(); i++)
        vec[i] = 0.0;
}

#define ROUND_DOWN(x, s) ((x) & ~((s)-1))
void assembly_vec()
{
    long n = 10;
    QVector<float> data(n);
    reset(data);

    qDebug() << data;
    array_addss_cpp(data.data(), 1.0, data.size());

    qDebug() << data;
    array_addss_iter(data.data(), 1.0, data.size());
    qDebug() << data;

    reset(data);
    array_addss_vect(data.data(), 1.0, data.size());
    qDebug() << data;

}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    assembly_ops();
    assembly_vec();

    return 0;
}
