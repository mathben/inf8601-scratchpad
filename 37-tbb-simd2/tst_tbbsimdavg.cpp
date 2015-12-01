#include <QString>
#include <QtTest>
#include <tbb/tbb.h>

class TbbSimdAvg : public QObject
{
    Q_OBJECT

public:
    TbbSimdAvg();

private Q_SLOTS:
    void testCase();
    void testCaseSerial();
    void testCaseTbb();
    void testCaseTbbSimd();
};

TbbSimdAvg::TbbSimdAvg()
{
}

void mean3(QVector<float> &m, const QVector<float> &x)
{
    for (int i = 1; i < x.size() - 1; i++) {
        m[i] = (x[i-1] + x[i] + x[i+1]) * (1/3.0f);
    }
}

void mean3_tbb(QVector<float> &m, const QVector<float> &x)
{
    tbb::parallel_for(tbb::blocked_range<uint>(1, x.size() - 1),
        [&](tbb::blocked_range<uint> &range) {
            for (uint i = range.begin(); i < range.end(); i++) {
                m[i] = (x[i-1] + x[i] + x[i+1]) * (1/3.0f);
            }
        }
    );
}

/* Auteur: Valentin Vendengeon */
void mean_simd_tbb(QVector<float> &m, const QVector<float> &x)
{
    float f = 1/3.0f;
    __m128 F = _mm_set_ps(f, f, f, f);
    tbb::parallel_for(tbb::blocked_range<uint>(0, (x.size() - 1) / 4),
        [&](tbb::blocked_range<uint> &range) {
            for (uint i = range.begin(); i < range.end(); i++) {
                __m128 V1 = _mm_loadu_ps(x.data() + 0 + i * 4);
                __m128 V2 = _mm_loadu_ps(x.data() + 1 + i * 4);
                __m128 V3 = _mm_loadu_ps(x.data() + 2 + i * 4);
                __m128 A = _mm_add_ps(V1, V2);
                __m128 B = _mm_add_ps(A, V3);
                __m128 C = _mm_mul_ps(B, F);
                _mm_storeu_ps(m.data() + 1 + i * 4, C);
            }
        }
    );
}

void TbbSimdAvg::testCase()
{
    int n = 18;
    QVector<float> in(n);
    QVector<float> out1(n);
    QVector<float> out2(n);
    QVector<float> out3(n);

    for (int i = 0; i < in.size(); i++) {
        in[i] = (i % 2 == 0) ? i : 0;
    }

    mean_simd_tbb(out1, in);
    mean3_tbb(out2, in);
    mean3(out3, in);

    qDebug() << in;
    qDebug() << out1;
    qDebug() << out2;
    qDebug() << out3;
}

void TbbSimdAvg::testCaseSerial()
{
    int n = 1E6;
    QVector<float> in(n);
    QVector<float> out1(n);

    QBENCHMARK {
        mean3(out1, in);
    }
}

void TbbSimdAvg::testCaseTbb()
{
    int n = 1E6;
    QVector<float> in(n);
    QVector<float> out1(n);

    QBENCHMARK {
        mean3_tbb(out1, in);
    }
}

void TbbSimdAvg::testCaseTbbSimd()
{
    int n = 1E6;
    QVector<float> in(n);
    QVector<float> out1(n);

    QBENCHMARK {
        mean_simd_tbb(out1, in);
    }
}

QTEST_APPLESS_MAIN(TbbSimdAvg)

#include "tst_tbbsimdavg.moc"
