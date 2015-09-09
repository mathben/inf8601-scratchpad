#include <QDebug>
#include <iostream>
#include <pthread.h>

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
        pthread_cond_wait(&ctx->cond, &ctx->mutex);

        if (ctx->quit) {
            pthread_mutex_unlock(&ctx->mutex);
            break;
        }
        if (!ctx->input.isEmpty()) {
            QString cmd = ctx->input.takeFirst();
            pthread_mutex_unlock(&ctx->mutex);
            qDebug() << "thread" << pthread_self() << "says" << cmd;
        }
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
        if (line.compare("quit") == 0) {
            ctx->quit = true;
            pthread_cond_broadcast(&ctx->cond);
            break;
        }

        pthread_mutex_lock(&ctx->mutex);
        ctx->input.append(line);
        pthread_mutex_unlock(&ctx->mutex);

        pthread_cond_signal(&ctx->cond);
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
