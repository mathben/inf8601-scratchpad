#include <QDebug>
#include <QElapsedTimer>
#include "tbb/tbb.h"

// compute peak FP
// normalize to peak

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int ncpus = tbb::task_scheduler_init::default_num_threads();
    qint64 base_size = (1 << 25); // sizeof(float) * 2^25 = 4 * 32 = 128MB
    qint64 size = base_size;
    QVector<float> v0, v1, v2, v3;

    QElapsedTimer time;
    QVector<int> ops = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
    QVector<int> cpus = {1, ncpus};

    v0.fill(3.1416, size);
    v1.fill(3.1416, size);
    v2.fill(0, size);
    v2.fill(0, size);

    for (int n : cpus) {
        tbb::task_scheduler_init sched(n);
        for (int op : ops) {
            double sum = 0;
            int max_repeat = 3;
            for (int repeat = 0; repeat < max_repeat; repeat++) {
                qint64 t1 = time.nsecsElapsed();
                tbb::parallel_for(tbb::blocked_range<int>(0, size / 4),
                    [&](tbb::blocked_range<int> &range) {
                        for (int i = range.begin(); i < range.end(); i++) {
                            int id = i * 4;
                            __m128 X = _mm_loadu_ps(v0.data() + id);
                            __m128 Y = _mm_loadu_ps(v1.data() + id);
                            for (int repeat = 0; repeat < op; repeat++) {
                                X = _mm_mul_ps(X, X);
                                Y = _mm_add_ps(Y, Y);
                            }
                            _mm_storeu_ps(v1.data() + id, X);
                            _mm_storeu_ps(v2.data() + id, Y);
                        }
                    }
                );
                sum += time.nsecsElapsed() - t1;
            }
            double mean = sum / max_repeat;
            double flops = (size * (op * 2)) / (mean / 1E9);
            qDebug() << flops;
        }
    }

    return 0;
}

