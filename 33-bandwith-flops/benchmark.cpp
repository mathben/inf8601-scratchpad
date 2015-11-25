#include "benchmark.h"

Benchmark::Benchmark(QObject *parent) :
    QObject(parent), m_repeat(1)
{
}

void Benchmark::run(Workload *item)
{
    qint64 min = 1E8;
    QElapsedTimer clock;
    int ncpus = tbb::task_scheduler_init::default_num_threads();
    QVector<qint64> samples;
    item->init();
    for (int cpus = 1; cpus <= ncpus; cpus++) {
        samples.clear();
        tbb::task_scheduler_init sched(cpus);
        item->before();
        qint64 elapsed = 0;
        qint64 iter = 0;
        while (elapsed < min || iter < m_repeat) {
            qint64 t1 = clock.nsecsElapsed();
            item->work();
            qint64 ns = clock.nsecsElapsed() - t1;
            samples.append(ns);
            elapsed += ns;
            iter++;
        }
        item->after();
        double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
        double mean = (sum / samples.size()) / item->m_size;
        //double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
        //double stdev = std::sqrt(sq_sum / v.size() - mean * mean);
        qDebug() << item->m_name << cpus << samples.size() << mean;
        m_means[item->m_name][cpus] = mean;
    }
    item->teardown();
}

void Benchmark::csv(QString &path)
{
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
