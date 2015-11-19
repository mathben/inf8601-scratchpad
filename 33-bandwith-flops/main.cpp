#include <QDebug>
#include "benchmark.h"
#include <xmmintrin.h>

struct BenchmarkItem {
    QString name;
    std::function<void ()> func;
    int size;
};

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int size = (1 << 26); // 128MB
    QVector<float> v(size);

    auto bw_load = [&]() {
        tbb::parallel_for(tbb::blocked_range<int>(0, size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile float x = v[i];
                    (void) x;
                }
            }
        );
    };

    auto bw_store = [&]() {
        tbb::parallel_for(tbb::blocked_range<int>(0, size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    v[i] = 42;
                }
            }
        );
    };

    __m128 ps = _mm_set1_ps(3.1416);
    auto bw_simd = [&]() {
        tbb::parallel_for(tbb::blocked_range<int>(0, size / 4),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    _mm_storeu_ps(v.data() + (i * 4), ps);
                }
            }
        );
    };

    float cst = 3.1416;
    int nflops = (1 << 26);
    auto fp_basic = [&]() {
        tbb::parallel_for(tbb::blocked_range<int>(0, nflops),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile float val = i * cst;
                    (void) val;
                }
            }
        );
    };

    __m128 cstv = _mm_set1_ps(cst);
    auto fp_simd = [&]() {
        tbb::parallel_for(tbb::blocked_range<int>(0, nflops / 4),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    __m128 local = _mm_set1_ps(cst);
                    volatile __m128 val = _mm_mul_ps(local, cstv);
                    (void) val;
                    (void) i;
                }
            }
        );
    };

    QVector<float> x(size);
    QVector<float> y(size);
    float a = 1.234;

    auto saxpy_basic = [&]() {
        tbb::parallel_for(tbb::blocked_range<int>(0, size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    y[i] = a * x[i] + y[i];
                }
            }
        );
    };

    __m128 va = _mm_set1_ps(a);
    auto saxpy_simd = [&]() {
        tbb::parallel_for(tbb::blocked_range<int>(0, size / 4),
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
    };

    QVector<BenchmarkItem> items = {
        { "bw_load", bw_load, size },
        { "bw_store", bw_store, size },
        { "bw_simd", bw_simd, size },
        { "fp_basic", fp_basic, nflops },
        { "fp_simd", fp_simd, nflops },
        { "saxpy_basic", saxpy_basic, size },
        { "saxpy_simd", saxpy_simd, size },
    };

    Benchmark bench;

    for (BenchmarkItem item : items) {
        bench.run(item.name, item.func, item.size);
    }

    QString out("results.csv");
    bench.csv(out);

    return 0;
}
