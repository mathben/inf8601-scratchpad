#include <QDebug>
#include <QVector>
#include "tbb/tbb.h"

/*
 * parallel implementation of memset()
 */

class MemSet {
public:
    MemSet(int *data): m_data(data) { }
    /*
     * The actual work is implemented into operator(), but the method has a body const.
     * It means we cannot change object members. They can be passed as raw pointers,
     * or marked mutable.
     */

private:
    int *m_data;
};

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int n = 100;
    int *array = new int[n];

    // 1- classic, explicit parallel_for

    // 2- lambda

    // 3- lambda without inner loop

    delete[] array;
}
