#include "manager.h"
#include "algo.h"

struct args {
    int id;
    Algo *algo;
};

static void *startHelper(void *arg)
{
    struct args *a = (struct args *) arg;
    a->algo->compute(a->id);
    return NULL;
}

Manager::Manager(QObject *parent) :
    QObject(parent),
    m_current(NULL),
    m_cores(2),
    m_running(false)
{
}

void Manager::start(Algo *algo)
{
    if (m_running) {
        stop();
    }
    m_current = algo;
    struct args a[m_cores];

    for (int i = 0; i < m_cores; i++) {
        a[i].id = i;
        a[i].algo = algo;
        pthread_create(&threads[i], NULL, startHelper, &a[i]);
    }
    m_running = true;
}

void Manager::stop()
{
    m_current->stop();
    for (int i = 0; i < m_cores; i++) {
        pthread_join(threads[i], NULL);
    }
    m_running = false;
}
