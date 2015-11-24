#include <QDebug>
#include "benchmark.h"
#include <xmmintrin.h>

struct BenchmarkItem {
    QString name;
    std::function<void ()> func;
    int size;
};

/** FIXME: template */
class Workload {
    Workload(int flops, int data, int size, int nvec) {
        m_ints.resize(nvec);
        m_longs.resize(nvec);
        m_floats.resize(nvec);
        m_doubles.resize(nvec);
        m_size = size;
        m_flops = flops;
        m_data = data;
    }
public:
    void init() {
        for (QVector<int> v: m_ints) { v.resize(m_size); qFill(v, 0); }
        for (QVector<long> v: m_longs) { v.resize(m_size); qFill(v, 0); }
        for (QVector<float> v: m_floats) { v.resize(m_size); qFill(v, 0); }
        for (QVector<double> v: m_doubles) { v.resize(m_size); qFill(v, 0); }
    }
    virtual void before() { }
    virtual void work() { }
    virtual void after() { }
protected:
    QString m_name;
    int m_size;
    QVector<QVector<int> > m_ints;
    QVector<QVector<long> > m_longs;
    QVector<QVector<float> > m_floats;
    QVector<QVector<double> > m_doubles;
    int m_flops;
    int m_data;
};

class LoadFloat : Workload {
public:
    void work() {
        QVector<float> v = m_floats[0];
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile float x = v[i];
                    (void) x;
                }
            }
        );
    }
};

class StoreFloat : Workload {
public:
    void work() {
        QVector<float> v = m_floats[0];
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    v[i] = 42;
                }
            }
        );
    }
};

class StoreFloatSIMD : Workload {
public:
    void work() {
        QVector<float> v = m_floats[0];
        __m128 ps = _mm_set1_ps(3.1416);
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size / 4),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    _mm_storeu_ps(v.data() + (i * 4), ps);
                }
            }
        );
    }
};

class MulFlops : Workload {
public:
    void work() {
        float cst = 3.1416;
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile float val = i * cst;
                    (void) val;
                }
            }
        );
    }
};

class DivFlops : Workload {
public:
    void work() {
        float cst = 3.1416;
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile float val = i / cst;
                    (void) val;
                }
            }
        );
    }
};

class SinFlops : Workload {
public:
    void work() {
        float cst = 3.1416;
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile float val = std::sin(i + cst);
                    (void) val;
                }
            }
        );
    }
};

class MulFlopsSIMD : Workload {
public:
    void work() {
        __m128 cstv = _mm_set1_ps(3.1416);
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size / 4),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    __m128 local = _mm_set1_ps(1.234);
                    volatile __m128 val = _mm_mul_ps(local, cstv);
                    (void) val;
                    (void) i;
                }
            }
        );
    }
};

class SAXPYBasic: Workload {
public:
    void work() {
        QVector<float> x = m_floats[0];
        QVector<float> y = m_floats[1];
        float a = 3.1416;
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    y[i] = a * x[i] + y[i];
                }
            }
        );
    }
};

class SAXPYSIMD: Workload {
public:
    void work() {
        QVector<float> x = m_floats[0];
        QVector<float> y = m_floats[1];
        __m128 va = _mm_set1_ps(3.1416);
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size / 4),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    int id = i * 4;
                    __m128 vx = _mm_loadu_ps(x.data() + id);
                    __m128 vy = _mm_loadu_ps(y.data() + id);
                    vx = _mm_mul_ps(va, vx);
                    vy = _mm_add_ps(vx, vy);
                    _mm_storeu_ps(y.data() + id, vy);
                }
            }
        );
    }
};

// call the constructor with correct parameters
// rate statistics: MB/s and flop/s (not only time)
// workload: matrix multiplication with blocking
// workload: loop fusion
// mul, div, sin

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    QVector<BenchmarkItem> items = {
        /*
        { "bw_load", bw_load, size },
        { "bw_store", bw_store, size },
        { "bw_simd", bw_simd, size },
        { "fp_basic", fp_basic, nflops },
        { "fp_simd", fp_simd, nflops },
        { "saxpy_basic", saxpy_basic, size },
        { "saxpy_simd", saxpy_simd, size },
        */
    };

    Benchmark bench;

    for (BenchmarkItem item : items) {
        bench.run(item.name, item.func, item.size);
    }

    QString out("results.csv");
    bench.csv(out);

    return 0;
}
