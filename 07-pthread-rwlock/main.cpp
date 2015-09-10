#include <QDebug>
#include <QtGlobal>
#include <QThread>
#include <pthread.h>
#include <limits>

/*
 * Force the compiler to access a variable
 *
 * #define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
 */
template<typename T>
inline T volatile &access_once(T &t) {
    return static_cast<T volatile &>(t);
}

int rand_range(int xmin, int xmax) {
    return (qrand() % (xmax - xmin + 1)) + xmin;
}

typedef struct work {
    int run;
    QMap<QString, int> *weather;
} work_t;

void *client(void *arg)
{
    work_t *item = static_cast<work_t*>(arg);

    while(item->run) {
        QString coldestCity;
        int coldestTemp = std::numeric_limits<int>::max();

        /* Take read lock */

        QMapIterator<QString, int> i(*item->weather);
        while(i.hasNext()) {
            i.next();
            if (i.value() < coldestTemp) {
                coldestTemp = i.value();
                coldestCity = i.key();
            }
        }
        qDebug() << "thread" << pthread_self() << "says coldest city is" << coldestCity << coldestTemp;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    static QMap<QString, int> weather {
        { "Montreal", 0 },
        { "Paris",    0 },
        { "Tokyo",    0 },
    };

    int n = 4;
    pthread_t threads[n];
    work_t work_item;

    /* Initialize variables */
    (void) threads;
    work_item.run = 1;

    for (int i = 0; i < n; i++) {
        /* Create threads */
    }

    /* Update weather info */
    for (int i = 0; i < 10; i++) {
        /* Take write lock */
        weather["Montreal"] = rand_range(-20, 10);
        weather["Paris"]    = rand_range(0, 20);
        weather["Tokyo"]    = rand_range(-5, 5);

        QThread::usleep(2);
    }

    access_once(work_item.run) = 0;

    for (int i = 0; i < n; i++) {
        /* Wait threads */
    }
    return 0;
}
