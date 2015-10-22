#include <QDebug>
#include <QString>

#define NB_BIT_CACHE 6
#define CACHE_LENGTH (1 << NB_BIT_CACHE) // 64-byte cache blocks

bool is_bleeding(void *addr, size_t size)
{
    ulong ula = (ulong) addr;
    ulong x0 = ula % CACHE_LENGTH; // the beginning of the cache
    ulong x1 = x0 + size - 1;
    bool bleed = (x1 >= CACHE_LENGTH);
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
    long g = 7;
    long h = 8;

    is_bleeding(&a, sizeof(long));
    is_bleeding(&b, sizeof(long));
    is_bleeding(&c, sizeof(long));
    is_bleeding(&d, sizeof(long));
    is_bleeding(&e, sizeof(long));
    is_bleeding(&f, sizeof(long));
    is_bleeding(&g, sizeof(long));
    is_bleeding(&h, sizeof(long));

    long buf = 9;
    ulong ulbuf = (ulong) &buf;
    // put zero last NB_BIT_CACHE bits
    ulbuf = ulbuf & (~0 << NB_BIT_CACHE);
    // force wrong alignment with CACHE_LENGTH
    long *foo = (long *) (ulbuf + (CACHE_LENGTH - sizeof(long) + 1) );
    qDebug() << "\nreal &" << &buf << "value" << buf ;
    qDebug() << "fake &" << QString("0x%1").arg(ulbuf, 0, 16) << "value" << *foo << "&foo " << foo;
    qDebug() << "Expect bleed";
    // pass pointer to wrong address alignment
    is_bleeding(foo, sizeof(long));

    qDebug() << "\nTest force aligned";
    long __attribute__((aligned(CACHE_LENGTH))) x = 42;
    long __attribute__((aligned(CACHE_LENGTH))) y = 42;
    long __attribute__((aligned(CACHE_LENGTH))) z = 42;
    is_bleeding(&x, sizeof(long));
    is_bleeding(&y, sizeof(long));
    is_bleeding(&z, sizeof(long));

    return 0;
}
