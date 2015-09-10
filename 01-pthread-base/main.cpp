#include <QDebug>

void *routine(void *arg)
{
    (void) arg;
    qDebug() << "routine" << pthread_self();
    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    /* Initialize variables */
    pthread_t thread1;
    pthread_t thread2;

    /* Create a thread */
    pthread_create(&thread1, NULL, routine, NULL);
    pthread_create(&thread2, NULL, routine, NULL);

    /* Wait the thread */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    qDebug() << "done" ;

    return 0;
}
