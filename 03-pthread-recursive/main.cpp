#include <QDebug>

void *thread_bar(void *arg)
{
    (void) arg;
    qDebug() << "bar";
    return 0;
}

void *thread_foo(void *arg)
{
    (void) arg;

    qDebug() << "foo  before";

    /* create bar and wait */
    pthread_t thread;
    pthread_create(&thread, NULL, thread_bar, NULL);
    pthread_join(thread, NULL);
    qDebug() << "foo  after";
    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    qDebug() << "main begin";

    /* create foo and wait */
    pthread_t thread;
    pthread_create(&thread, NULL, thread_foo, NULL);
    pthread_join(thread, NULL);

    qDebug() << "main done";
    return 0;
}
