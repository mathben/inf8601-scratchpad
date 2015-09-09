#include <QDebug>
#include <pthread.h>

void *routine(void *arg)
{
    (void) arg;

    qDebug() << "hello!" << arg << *((int *)arg);
    return (void *) 123;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    pthread_t thread;
    pthread_attr_t attr;
    int arg = 42;
    ulong ret;

    int err = pthread_create(&thread, &attr, routine, &arg);
    qDebug() << "ptherad_create=" << err;

    err = pthread_join(thread, (void **)&ret);
    qDebug() << "ptherad_join=" << err;

    return 0;
}
