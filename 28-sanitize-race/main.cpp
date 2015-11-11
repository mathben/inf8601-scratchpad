#include <QDebug>
#include <pthread.h>

void *worker(void *args) {
    int *x = (int *) args;
    *x = *x + 1;
    return NULL;
}


int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int var = 0;
    int n = 2;
    pthread_t t[n];

    for (int i = 0; i < n; i++) {
        pthread_create(&t[i], NULL, worker, &var);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(t[n], NULL);
    }

    return 0;
}
