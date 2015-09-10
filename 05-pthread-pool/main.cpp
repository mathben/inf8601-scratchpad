#include <QDebug>
#include <QThread>
#include <iostream>
#include <pthread.h>
#include <cassert>

using namespace std;

// cool stuff: https://github.com/facebook/folly

/*
 * Force the compiler to access a variable
 *
 * #define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
 */
template<typename T>
inline T volatile &access_once(T &t) {
    return static_cast<T volatile &>(t);
}

typedef struct ctx {
    /* Members */
    volatile int quit;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    QVector<QString> commandes;
} ctx_t;

void *worker(void *arg)
{
    ctx_t *ctx = static_cast<ctx_t*>(arg);
    /* Take work and execute */
    // attendre un element

    for(;;) {

        pthread_mutex_lock(&ctx->mutex);

        // verifier si travail a faire

        pthread_cond_wait(&ctx->cond, &ctx->mutex);

        if (access_once(ctx->quit)) {
            pthread_mutex_unlock(&ctx->mutex);
            break;
        }

        // section critique
        assert(ctx->commandes.size() > 0);
        QString cmd = ctx->commandes.takeFirst();

        pthread_mutex_unlock(&ctx->mutex);

        qDebug() << "thread" << pthread_self() << cmd;

        QThread::msleep(1000);

    }

    qDebug() << "bye" << pthread_self();
    return 0;
}

void loop(ctx_t *ctx)
{
    (void) ctx;
    QTextStream stream(stdin);
    QString line;

    for (;;) {
        cout << "> ";
        line = stream.readLine().trimmed();
        if (line.compare("quit") == 0) {
            access_once(ctx->quit) = 1;
            /* Signal to quit */
            pthread_cond_broadcast(&ctx->cond);
            break;
        }

        pthread_mutex_lock(&ctx->mutex);
        ctx->commandes.append(line);
        pthread_mutex_unlock(&ctx->mutex);

        /* Add command and signal */
        pthread_cond_signal(&ctx->cond);
    }
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    /* Initialize variables */
    int n = 4;
    pthread_t threads[n];
    ctx_t ctx;

    access_once(ctx.quit) = 0;
    pthread_mutex_init(&ctx.mutex, NULL);
    pthread_cond_init(&ctx.cond, NULL);

    for (int i = 0; i < n; i++) {
        /* Create threads */
        pthread_create(&threads[i], NULL, worker, &ctx);
    }

    /* Main loop */
    loop(&ctx);

    /* Wait threads */
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
