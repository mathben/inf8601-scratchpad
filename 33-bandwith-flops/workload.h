#ifndef WORKLOAD_H
#define WORKLOAD_H

#include <QString>
#include <QVector>
#include <QDebug>

class Workload {
public:
    Workload(const char *name, int flops, int data, int nvec);

    void init();
    void teardown();
    virtual void before() { }
    virtual void work() { }
    virtual void after() { }

    QString m_name;
    int m_size;
    QVector<QVector<int> > m_ints;
    QVector<QVector<long> > m_longs;
    QVector<QVector<float> > m_floats;
    QVector<QVector<double> > m_doubles;
    int m_flops;
    int m_bytes;
    int m_nvec;
};

#endif // WORKLOAD_H
