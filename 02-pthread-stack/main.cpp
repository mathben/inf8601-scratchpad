#include <QDebug>
#include <pthread.h>

typedef struct thread_arg {
    ulong rsp;
} arg_t;

void *thread(void *args)
{
    register ulong rsp asm("rsp");
    arg_t *arg = (arg_t *) args;
    arg->rsp = rsp;
    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int n = 4;
    pthread_t t[n];
    arg_t args[n];

    for (int i = 0; i < n; i++) {
        pthread_attr_t attr;
        size_t ss = (1 << 15);
        pthread_attr_init(&attr);
        pthread_attr_setstacksize(&attr, ss);
        pthread_create(&t[i], &attr, thread, &args[i]);
        pthread_attr_destroy(&attr);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(t[i], NULL);
        qDebug() << i << (void *)args[i].rsp;
    }

    for (int i = 0; i < n - 1; i++) {
        ulong space = args[i].rsp - args[i + 1].rsp;
        qDebug() << "space" << space;
    }

    return 0;
}
