#include <QDebug>
#include <pthread.h>

class Temperature {
public:
    Temperature(int i) : m_temp(i) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_mutex, &attr);
    }

    void set(int temp) {
        /* Protect */
        pthread_mutex_lock(&m_mutex);
        m_temp = temp;
        pthread_mutex_unlock(&m_mutex);
    }

    int get() {
        /* Protect */
        pthread_mutex_lock(&m_mutex);
        int temp = m_temp;
        pthread_mutex_unlock(&m_mutex);
        return temp;
    }

    void increment() {
        pthread_mutex_lock(&m_mutex);
        set(m_temp + 1);
        pthread_mutex_unlock(&m_mutex);
    }

private:
    pthread_mutex_t m_mutex;
    int m_temp;
};

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    Temperature t(42);
    qDebug() << "temperature before" << t.get();
    t.increment();
    qDebug() << "temperature after " << t.get();
}
