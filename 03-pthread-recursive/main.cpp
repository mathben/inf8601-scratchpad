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

    qDebug() << "foo  after";
    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    qDebug() << "main begin";

    /* create foo and wait */

    qDebug() << "main done";
    return 0;
}
