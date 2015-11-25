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
    qint64 base_size = item->m_size;
    for (int cpus = 1; cpus <= ncpus; cpus++) {
        item->m_size = base_size * cpus;
        item->init();
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
        double mean = (sum / samples.size());

        double ts = mean / item->m_size;
        double bw = item->m_bytes * item->m_size / (mean / 1E9);
        double fp = item->m_flops * item->m_size / (mean / 1E9);

        //double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
        //double stdev = std::sqrt(sq_sum / v.size() - mean * mean);
        qDebug() << item->m_name << cpus << samples.size() << mean;
        m_ts[item->m_name][cpus] = ts;
        m_bw[item->m_name][cpus] = bw;
        m_fp[item->m_name][cpus] = fp;
    }
    item->teardown();
}

void Benchmark::csv(const char *path, QMap<QString, QMap<int, double> > &data)
{
    QFile file(path);
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&file);
        out << "cores,";
        for (const QString name : data.keys()) {
            out << name << ",";
        }
        out << "\n";

        QList<int> ncpus = data[data.firstKey()].keys();
        for (const int i : ncpus) {
            out << i << ",";
            for (const QString name : data.keys()) {
                out << data[name][i] << ",";
            }
            out << "\n";
        }
    }
}
