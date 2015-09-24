#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QVector>
#include "algo.h"

class Manager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int cores READ cores WRITE setCores NOTIFY coresChanged)

public:
    explicit Manager(QObject *parent = 0);
    void start(Algo *algo);
    void stop();
    QVector<Algo> algos();

    void setCores(int cores) {
        m_cores = cores;
        emit coresChanged(m_cores);
    }
    int cores() const {
        return m_cores;
    }

signals:
    void coresChanged(int);

public slots:

private:
    QVector<pthread_t> threads;
    Algo *m_current;
    int m_cores;
    bool m_running;
};

#endif // MANAGER_H
