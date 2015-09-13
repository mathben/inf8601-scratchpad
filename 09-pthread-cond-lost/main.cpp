#include <QDebug>
#include <pthread.h>

/*
 * Simulate a lost signal. This program will indefinitely block on
 * pthread_cond_wait(), because the signal is sent outside the
 * critical region protected by the mutex, and the signal is
 * not queued.
 */

pthread_barrier_t barrier;
pthread_mutex_t mutex;
pthread_cond_t cond;

int var = 0;

void *routine(void *arg)
{
    (void) arg;


    /* thread is alive */
    qDebug() << "thread running";
    pthread_barrier_wait(&barrier);

    /* send signal */
    pthread_barrier_wait(&barrier);

    qDebug() << "pthread_mutex_lock";
    pthread_mutex_lock(&mutex);
    /*
     * Wait for the signal, but it was already delivered, and
     * will never be received. To receive the signal, the
     * thread must wait for it *before* it is sent.
     *
     * To avoid this issue: the thread calling pthread_cond_broadcast()
     * must hold the mutex associated with the condition.
     */
    qDebug() << "pthread_cond_wait";
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    pthread_t thread;

    pthread_barrier_init(&barrier, NULL, 2);
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&thread, NULL, routine, NULL);

    /* thread is alive */
    pthread_barrier_wait(&barrier);

    /* send signal */
    qDebug() << "pthread_cond_broadcast";
    pthread_cond_broadcast(&cond);
    pthread_barrier_wait(&barrier);

    qDebug() << "pthread_join";
    pthread_join(thread, NULL);

    return 0;
}
