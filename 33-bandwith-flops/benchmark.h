#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <QObject>
#include <QVector>
#include <QElapsedTimer>
#include <QMap>
#include <functional>
#include <tbb/tbb.h>
#include <unistd.h>
#include <QDebug>
#include <QFile>

class Benchmark : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int m_repeat READ repeat WRITE setRepeat)

public:
    explicit Benchmark(QObject *parent = 0);

    void run(QString &name, std::function<void ()> func, int size)
    {
        QElapsedTimer clock;
        int ncpus = tbb::task_scheduler_init::default_num_threads();
        QVector<qint64> samples(m_repeat);
        for (int cpus = 1; cpus <= ncpus; cpus++) {
            std::fill(samples.begin(), samples.end(), 0);
            tbb::task_scheduler_init sched(cpus);
            for (int i = 0; i < m_repeat; i++) {
                qint64 t1 = clock.nsecsElapsed();
                func();
                qint64 t2 = clock.nsecsElapsed();
                samples[i] = t2 - t1;
            }
            double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
            double mean = (sum / samples.size()) / size;
            /*
            double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
            double stdev = std::sqrt(sq_sum / v.size() - mean * mean);
            */
            qDebug() << name << cpus << mean;
            m_means[name][cpus] = mean;
        }
    }

    int repeat() { return m_repeat; }
    void setRepeat(int repeat) { m_repeat = repeat; }
    QMap<QString, QMap<int, double> > getMeans() { return m_means; }

    void csv(QString &path) {
        int ncpus = tbb::task_scheduler_init::default_num_threads();
        QFile data(path);
        if (data.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream out(&data);
            out << "cores,";
            for (QString name : m_means.keys()) {
                out << name << ",";
            }
            out << "\n";
            for (int i = 1; i <= ncpus; i++) {
                out << i << ",";
                for (QString name : m_means.keys()) {
                    out << m_means[name][i] << ",";
                }
                out << "\n";
            }
        }
    }

signals:

public slots:

private:
    int m_repeat;
    QMap<QString, QMap<int, double> > m_means;
};

#endif // BENCHMARK_H
