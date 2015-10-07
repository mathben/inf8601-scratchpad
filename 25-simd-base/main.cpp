#include <QDebug>
#include <QString>
#include <cstdio>
#include <cassert>

#include "ops.h"
#include "vec.h"

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

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

    long n = 10;
    QVector<long> data(n);
    for (int i = 0; i < n; i++)
        data[i] = i;

    qDebug() << data;
    array_add_scalar_iter(data.data(), 42, data.size());
    qDebug() << data;
    return 0;
}
