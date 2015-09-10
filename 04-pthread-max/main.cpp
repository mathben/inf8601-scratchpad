#include <QDebug>

void *bidon(void *arg)
{
    (void) arg;
    /* Wait until signal */
    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    for (;;) {
        /* Create threads */
    }

    /* Send signal to return and wait */

    return 0;
}
