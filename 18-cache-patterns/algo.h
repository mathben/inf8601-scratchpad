#ifndef ALGO_H
#define ALGO_H

#include <QObject>

class Algo : public QObject
{
    Q_OBJECT
public:
    explicit Algo(QObject *parent = 0);
    virtual void compute(int id) = 0;
    void stop() {
        m_stop = true;
    }
signals:

public slots:

private:
    volatile bool m_stop;
};

#endif // ALGO_H
