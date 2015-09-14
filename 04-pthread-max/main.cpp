#include <QDebug>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t mutex;

void *bidon(void *arg)
{
    (void) arg;
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    QVector<pthread_t> threads;

    for (;;) {
        pthread_t t;
        int ret = pthread_create(&t, NULL, bidon, NULL);
        if (ret == 0) {
            threads.append(t);
        } else {
            qDebug() << "max thread" << threads.size();
            break;
        }
    }

    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    foreach (pthread_t thread, threads) {
        pthread_join(thread, NULL);
    }
    return 0;
}
