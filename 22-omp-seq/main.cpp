#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <math.h>
#include <iostream>

QString gnuplot(
"set terminal png\n"
"set output '/tmp/seq.png'\n"
"set grid\n"
"set title 'Signal according to time'\n"
"set xlabel 'Amplitude'\n"
"set ylabel 'Time (s)'\n"
"plot '/tmp/seq.data' u 1 w lp, '/tmp/seq.data' u 2 w lp\n");

void write_data(QVector<float> &orig, QVector<float> &norm)
{
    QFile data;
    data.setFileName("/tmp/seq.data");
    data.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream out(&data);
    for (int i = 0; i < orig.length(); i++) {
        out << QString("%1 %2\n").arg(orig[i]).arg(norm[i]);
    }
    data.close();

    QString plotpath = "/tmp/seq.gnuplot";
    QFile plot;
    plot.setFileName(plotpath);
    plot.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream out2(&plot);
    out2 << gnuplot;
    plot.close();

    QProcess process;
    process.start("gnuplot", QStringList() << plotpath);
    process.waitForFinished();
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    // variables
    int n = 1000;
    float max = 0.0;
    int i = 0;
    float p = 0.1;
    float t = 0.0;
    QVector<float> orig(n);
    QVector<float> norm(n);

    // init sound-like data
    #pragma omp parallel for
    for (i = 0; i < n; i++) {
        orig[i] = std::cos((t + 0.1) * 0.1) +
                  std::cos((t + 0.2) * 0.2) +
                  std::cos((t + 0.3) * 0.3);
        t += p;
    }

    // detect the max value
    for (i = 0; i < n; i++) {
        max = std::max(max, std::abs(orig[i]));
    }

    qDebug() << "max=" << max;

    // normalize
    for (i = 0; i < n; i++) {
        norm[i] = orig[i] / max;
    }

    // verify the max value
    max = 0.0;
    for (i = 0; i < n; i++) {
        max = std::max(max, std::abs(norm[i]));
    }

    qDebug() << "normalized max=" << max;

    write_data(orig, norm);

    return 0;
}
