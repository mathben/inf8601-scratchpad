#include <QDebug>
#include <QString>
#include <cstdio>
#include <cassert>

#include "mmx.h"

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    hello();

    std::printf("%X\n", add2(1, 2));

    int cafebabe1 = add10_v1(0xCAFE0000, 0, 0, 0, 0, 0, 0, 0, 0, 0x0000BABE);
    int cafebabe2 = add10_v2(0xCAFE0000, 0, 0, 0, 0, 0, 0, 0, 0, 0x0000BABE);
    std::printf("%X\n", cafebabe1);
    std::printf("%X\n", cafebabe2);

    return 0;
}
