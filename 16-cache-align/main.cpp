#include <QDebug>
#include <QString>

bool is_bleeding(void *addr, size_t size)
{
    ulong ula = (ulong) addr;
    ulong x0 = ula % 64;
    ulong x1 = x0 + size - 1;
    bool bleed = (x1 >= 64);
    qDebug() << addr << x0 << x1 << bleed;
    return bleed;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    long a = 1;
    long b = 2;
    long c = 3;
    long d = 4;
    long e = 5;
    long f = 6;

    is_bleeding(&a, sizeof(long));
    is_bleeding(&b, sizeof(long));
    is_bleeding(&c, sizeof(long));
    is_bleeding(&d, sizeof(long));
    is_bleeding(&e, sizeof(long));
    is_bleeding(&f, sizeof(long));

    char* buf = new char[512];
    ulong ulbuf = (ulong) buf;
    ulbuf = ulbuf & (~0 << 6);
    long *foo = (long *) (ulbuf + (64 - 4));
    is_bleeding(foo, sizeof(long));

    long __attribute__((__aligned__(64))) x = 42;
    long __attribute__((__aligned__(64))) y = 42;
    long __attribute__((__aligned__(64))) z = 42;
    is_bleeding(&x, sizeof(long));
    is_bleeding(&y, sizeof(long));
    is_bleeding(&z, sizeof(long));

    delete[] buf;
    return 0;
}
