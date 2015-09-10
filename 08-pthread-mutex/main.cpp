#include <QDebug>
#include <pthread.h>

class Temperature {
public:
    Temperature(int i) : m_temp(i) {

    }

    void set(int temp) {
        /* Protect */
        m_temp = temp;
    }

    int get() {
        /* Protect */
        return m_temp;
    }

    void increment() {
        set(m_temp + 1);
    }

private:
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
