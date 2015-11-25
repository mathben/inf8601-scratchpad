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

#include "workload.h"

class Benchmark : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int m_repeat READ repeat WRITE setRepeat)

public:
    explicit Benchmark(QObject *parent = 0);

    void run(Workload *item);

    int repeat() { return m_repeat; }
    void setRepeat(int repeat) { m_repeat = repeat; }
    QMap<QString, QMap<int, double> > getMeans() { return m_means; }
    void csv(QString &path);

signals:

public slots:

private:
    int m_repeat;
    QMap<QString, QMap<int, double> > m_means;
};

#endif // BENCHMARK_H
