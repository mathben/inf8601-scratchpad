#include <QDebug>

pthread_mutex_t mutex;
pthread_cond_t cond;

void *bidon(void *arg)
{
    (void) arg;
    /* Wait until signal */

    pthread_mutex_lock(&mutex);
    // verrouillé
    pthread_cond_wait(&cond, &mutex);
    // verrouillé
    pthread_mutex_unlock(&mutex);

    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    QVector<pthread_t> threads;

    for (;;) {
        pthread_t current;
        int ret = pthread_create(&current, NULL, bidon, NULL);

        if (ret != 0) {
            break;
        } else {
            threads.append(current);
        }
    }

    /* Send signal to return and wait */
    pthread_cond_broadcast(&cond);
    //pthread_cond_signal(&cond);

    for (const pthread_t &thread: threads) {
        pthread_join(thread, NULL);
    }

    qDebug() << "max threads" << threads.size();

    return 0;
}
