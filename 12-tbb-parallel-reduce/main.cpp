#include <QDebug>
#include "tbb/tbb.h"

/*
 * Source: https://software.intel.com/fr-fr/node/506154
 */

class Sum {
public:
    // default constructor
    Sum(QVector<int>& data) : m_value(0), m_data(data) { }

    // tbb::split constructor: called to make copies of the object for each thread
    // in this case, the sum of a range starts at zero

    // implements operator() to reduce the provided range
    // the state of the object is updated

    // join() collapsed the result of two consecutive ranges

    int m_value;
    QVector<int>& m_data;
};

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int sum = 0;
    int n = 100;
    QVector<int> v(n);

    int exp = 0;
    for (int i = 0; i <n; i++) {
        v[i] = i;
        exp += i;
    }

    // classic parallel_reduce

    qDebug() << "exp" << exp << "act" << sum;


    // lambda
    /*
    sum = tbb::parallel_reduce(
        // global range to process
        // initial value

        // first lambda: compute the reduce for a subrange

        // second lambda: merge range results

    );
    */

    qDebug() << "exp" << exp << "act" << sum;

    return 0;
}
