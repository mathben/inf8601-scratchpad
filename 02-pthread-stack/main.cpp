#include <QDebug>
#include <pthread.h>

typedef struct thread_arg {
    ulong rsp;
} arg_t;

void *thread(void *args)
{
    (void) args;
    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int n = 4;
    pthread_t t[n];
    arg_t args[n];

    for (int i = 0; i < n; i++) {
        /* Set thread attribute */
        pthread_create(&t[i], NULL, thread, &args[i]);
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
