#include <QDebug>
#include <QVector>
#include <QElapsedTimer>
#include <math.h>
#include <cassert>
#include "tbb/tbb.h"

static void *worker(void *arg);

/* Basic Linear Algebra Subprograms (BLAS)
 * SAXPY: single-precision scale vector x by scalar a and adds to y
 * example: mix two sound channels
 */
class saxpy {
public:
    saxpy() : a(1.0), p(0.1), x(10), y(10) { }
    saxpy(int _len, float _a, float _p) :
        a(_a), p(_p), x(_len), y(_len) { }

    void reset() {
        float t = 0;
        for (int i = 0; i < x.size(); i++) {
            // sin(x)^2 + cos(x)^2 = 1
            x[i] = std::cos(t) * std::cos(t);
            y[i] = std::sin(t) * std::sin(t);
            t += p;
        }
    }

    void dump() {
        if (y.size() <= 10)
            qDebug() << y;
    }

    float a;
    float p;
    QVector<float> x;
    QVector<float> y;
};

typedef struct args {
    int id;
    int nr_thread;
    saxpy *data;
} args_t;

static void *worker(void *arg)
{
    args_t *m = static_cast<args_t*>(arg);

    int begin = (m->id)     * m->data->x.size() / m->nr_thread;
    int end   = (m->id + 1) * m->data->x.size() / m->nr_thread;

    saxpy& sax = *m->data;
    for (int i = begin; i < end; i++) {
        sax.y[i] = sax.a * sax.x[i] + sax.y[i];
    }

    return 0;
}

void do_serial(saxpy& sax)
{
    for (int i = 0; i < sax.x.size(); i++) {
        sax.y[i] = sax.a * sax.x[i] + sax.y[i];
    }
}

void do_pthread(saxpy *sax)
{
    int n = 4;
    pthread_t thread[n];
    args_t args[n];

    for (int i = 0; i < n; i++) {
        args[i].id = i;
        args[i].nr_thread = n;
        args[i].data = sax;
        pthread_create(&thread[i], NULL, worker, &args[i]);
    }

    for (int i = 0; i < n; i++)
        pthread_join(thread[i], NULL);
}

void do_tbb(saxpy& sax)
{
    // implements tbb::parallel_for using lambda

}

double elapsed(std::function<void ()> func)
{
    // measure the time taken by func
    return 0.0;
}

void print_result(QString name, double reference, double actual)
{
    qDebug() << QString("%1 %2s (%3x)")
                .arg(name, -12, QLatin1Char(' '))
                .arg(actual, 0, 'f', 6)
                .arg(reference / actual, 0, 'f', 2);
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int size = 1000000;

    saxpy sax(size, 1.0, 0.1);
    sax.reset();

    double e_serial = elapsed([&] () { do_serial(sax); });
    sax.reset();

    double e_pthread = elapsed([&] () { do_pthread(&sax); });
    assert(std::abs(sax.y.at(0) - 1.0) < 0.001);
    sax.reset();

    double e_tbb = elapsed([&] () { do_tbb(sax); });
    assert(std::abs(sax.y.at(0) - 1.0) < 0.001);

    print_result("serial", e_serial, e_serial);
    print_result("pthread", e_serial, e_pthread);
    print_result("tbb", e_serial, e_tbb);

    return 0;
}
