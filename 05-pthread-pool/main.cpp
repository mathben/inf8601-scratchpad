#include <QDebug>
#include <iostream>
#include <pthread.h>

using namespace std;

typedef struct ctx {
    /* Members */
} ctx_t;

void *thread(void *arg)
{
    (void) arg;
    /* Take work and execute */
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

            /* Signal to quit */

            break;
        }

        /* Add command and signal */
    }
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    /* Initialize variables */
    int n = 4;

    for (int i = 0; i < n; i++) {
        /* Create threads */
    }

    /* Main loop */
    loop(0);

    /* Wait threads */
    for (int i = 0; i < n; i++) {
    }

    return 0;
}
