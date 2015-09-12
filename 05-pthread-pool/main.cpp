#include <QDebug>
#include <iostream>
#include <pthread.h>
#include <cassert>
#include <QThread>

using namespace std;

typedef struct ctx {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    QVector<QString> input;
    bool quit;
} ctx_t;

void *thread(void *arg)
{
    ctx_t *ctx = static_cast<ctx_t*>(arg);

    for (;;) {
        pthread_mutex_lock(&ctx->mutex);

        /*
         * Wait until a command is available, or quit is requested.
         * If there is a command, don't wait and process it.
         */
        while (!ctx->quit && ctx->input.isEmpty()) {
            /*
             * Just before blocking, pthread_cond_wait unlocks the mutex and
             * the mutex is locked again when pthread_cond_wait() returns.
             */
            pthread_cond_wait(&ctx->cond, &ctx->mutex);
        }

        if (ctx->quit) {
            pthread_mutex_unlock(&ctx->mutex);
            break;
        }

        /* There should be a command */
        assert(!ctx->input.isEmpty());

        /* Take the command and release the lock ASAP */
        QString cmd = ctx->input.takeFirst();
        pthread_mutex_unlock(&ctx->mutex);

        /* Process the command, relax */
        qDebug() << "thread" << pthread_self() << "process" << cmd;
        QThread::msleep(10000);
        qDebug() << "thread" << pthread_self() << "done" << cmd;
    }
    return 0;
}

void loop(ctx_t *ctx)
{
    QTextStream stream(stdin);
    QString line;

    for (;;) {
        cout << "> ";
        line = stream.readLine().trimmed();

        /* Modification to shared structure requires the lock */
        pthread_mutex_lock(&ctx->mutex);

        if (line.compare("quit") == 0) {
            ctx->quit = true;
            /* signal before unlock */
            pthread_cond_broadcast(&ctx->cond);
            pthread_mutex_unlock(&ctx->mutex);
            break;
        }

        ctx->input.append(line);
        qDebug() << ctx->input;

        /* signal before unlock */
        pthread_cond_signal(&ctx->cond);
        pthread_mutex_unlock(&ctx->mutex);
    }
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int n = 4;
    pthread_t threads[n];
    ctx_t ctx;

    ctx.quit = false;
    pthread_mutex_init(&ctx.mutex, NULL);
    pthread_cond_init(&ctx.cond, NULL);

    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, thread, &ctx);
    }

    loop(&ctx);

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
