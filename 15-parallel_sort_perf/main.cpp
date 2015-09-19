#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>

#include <QDebug>
#include <QElapsedTimer>

#include <tbb/parallel_for.h>
#include <tbb/parallel_sort.h>

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    const int NB_INTS = 1000000;

    std::vector<int> ints(NB_INTS);
    ints.reserve(NB_INTS);
    for (int i = 0; i < NB_INTS; ++i)
    {
        ints[i] = i;
    }


    std::vector<double> acceleration;
    for (int i = 0; i < 10; i++) {
        std::random_shuffle(ints.begin(), ints.end());

        QElapsedTimer timer;

        timer.start();
        std::sort(ints.begin(), ints.end());
        double t1 = timer.nsecsElapsed() / 1e9;
        qDebug() << "Serial: " << t1;

        std::random_shuffle(ints.begin(), ints.end());

        timer.restart();
        tbb::parallel_sort(ints.begin(), ints.end());
        double t2 = timer.nsecsElapsed() / 1e9;
        qDebug() << "parallel_sort: " << t2;

        double speedup = t1 / t2;
        qDebug() << "speedup" << speedup;
        acceleration.push_back(speedup);
    }

    double sum = 0.0;
    for (double item: acceleration) {
        sum = sum + item;
    }

    double moyenne = sum / acceleration.size();
    qDebug() << moyenne;

    return 0;
}
