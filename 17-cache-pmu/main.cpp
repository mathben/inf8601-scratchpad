#include <QDebug>
#include <linux/perf_event.h>
#include <sys/syscall.h>
#include <unistd.h>

/* perf_event_open syscall wrapper */
static long
sys_perf_event_open(struct perf_event_attr *hw_event,
                    pid_t pid, int cpu, int group_fd, unsigned long flags)
{
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

/* gettid syscall wrapper */
static inline pid_t gettid()
{
    return syscall(SYS_gettid);
}

void write_linear(std::vector<int> &v)
{
    for (uint j = 0; j < v.size(); j++) {
        v[j] = j;
    }
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    perf_event_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.size = sizeof(attr);
    attr.type = PERF_TYPE_HARDWARE;
    //attr.config = PERF_COUNT_HW_CACHE_REFERENCES;
    //attr.config = PERF_COUNT_HW_CACHE_MISSES;
    //attr.config = PERF_COUNT_HW_CPU_CYCLES;
    attr.config = PERF_COUNT_HW_INSTRUCTIONS;
    attr.inherit = 1;
    attr.disabled = 0;

    pid_t tid = gettid();
    int fd = sys_perf_event_open(&attr, tid, -1, -1, 0);

    int samples = 10;
    long sum = 0;

    int len = 1000;
    std::vector<int> v(len);

    for (int i = 0; i < samples; i++) {
        long v1, v2;
        int ret = 0;
        ret = read(fd, &v1, sizeof(v1));

        write_linear(v);

        ret = read(fd, &v2, sizeof(v2));

        long delta = v2 - v1;
        qDebug() << "ret" << ret << "value" << delta;
        sum += delta;
    }
    qDebug() << v[0];
    qDebug() << "\n  avg value" << sum / samples;
    close(fd);
    return 0;
}
