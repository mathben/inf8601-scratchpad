#include <QDebug>
#include <QVector>
#include <QMap>
#include <exception>

#ifdef __APPLE__
    #include "OpenCL/opencl.h"
#else
    #include "CL/cl.h"
#endif

struct OpenCL {
public:
    OpenCL() { }

    void init() {
        /* get the number of platforms */
        cl_uint nrPlatforms = 0;
        clGetPlatformIDs(0, nullptr, &nrPlatforms);

        if (nrPlatforms == 0)
            throw std::runtime_error("No OpenCL platform");

        /* get the actual platform ids */
        m_platforms.resize(nrPlatforms);
        clGetPlatformIDs(nrPlatforms, m_platforms.data(), nullptr);

        /* devices */
        for(const auto id: m_platforms) {
            cl_uint nDev = 0;
            clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, 0, nullptr, &nDev);
            QVector<cl_device_id> deviceIDs(nDev);
            clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, nDev, deviceIDs.data(), nullptr);
            for (const auto dev: deviceIDs) {
                m_devices.insert(id, dev);
            }
        }
        if (m_devices.size() == 0)
            throw std::runtime_error("No OpenCL context");

        qDebug() << m_platforms;
        qDebug() << m_devices;

    }

    cl_platform_id get_platform() {
        return m_platforms.first();
    }

    cl_device_id get_device() {
        return m_devices[get_platform()];
    }

    QVector<cl_platform_id> m_platforms;
    QMap<cl_platform_id, cl_device_id> m_devices;
};

#define MULTI_LINE_STRING(...) #__VA_ARGS__
class SaxpyCL {

    /* the kernel string */
    const char *kernel_text = MULTI_LINE_STRING(
        __kernel void SAXPY (__global float* x,
                             __global float* y,
                             __global float* z,
                             float a) {
            const int i = get_global_id (0);
            z[i] = a * x[i] + y[i];
        }
    );

public:
    SaxpyCL() { }
    ~SaxpyCL() {
        close();
    }

    void init(cl_device_id dev) {
        cl_int ret;
        m_ctx       = clCreateContext(nullptr, 1, &dev, nullptr, nullptr, &ret);
        m_prog      = clCreateProgramWithSource(m_ctx, 1, &kernel_text, nullptr, nullptr);
        ret         = clBuildProgram(m_prog, 0, nullptr, "", nullptr, nullptr);
        m_kernel    = clCreateKernel(m_prog, "SAXPY", &ret);
        m_queue     = clCreateCommandQueue(m_ctx, dev, 0, &ret);
    }

    void compute(float a, QVector<float> &x, QVector<float> &y, QVector<float> &z) {
        cl_int ret;
        size_t size = x.size() * sizeof(float);
        size_t work_size[] = { size };

        cl_mem cl_x = clCreateBuffer(m_ctx, CL_MEM_READ_ONLY, size, nullptr, &ret);
        cl_mem cl_y = clCreateBuffer(m_ctx, CL_MEM_READ_ONLY, size, nullptr, &ret);
        cl_mem cl_z = clCreateBuffer(m_ctx, CL_MEM_WRITE_ONLY, size, nullptr, &ret);

        clEnqueueWriteBuffer(m_queue, cl_x, CL_FALSE, 0, size, x.data(), 0, nullptr, nullptr);
        clEnqueueWriteBuffer(m_queue, cl_y, CL_FALSE, 0, size, y.data(), 0, nullptr, nullptr);

        clSetKernelArg(m_kernel, 0, sizeof(cl_mem), &cl_x);
        clSetKernelArg(m_kernel, 1, sizeof(cl_mem), &cl_y);
        clSetKernelArg(m_kernel, 2, sizeof(cl_mem), &cl_z);
        clSetKernelArg(m_kernel, 3, sizeof(float), &a);

        clEnqueueNDRangeKernel(m_queue, m_kernel, 1, nullptr, work_size, nullptr, 0, nullptr, nullptr);
        clEnqueueReadBuffer(m_queue, cl_z, CL_TRUE, 0, size, z.data(), 0, nullptr, nullptr);

        clReleaseMemObject(cl_x);
        clReleaseMemObject(cl_y);
        clReleaseMemObject(cl_z);
    }

    void close() {
        clReleaseCommandQueue(m_queue);
        clReleaseContext(m_ctx);
    }

    OpenCL m_ocl;
    cl_context m_ctx;
    cl_program m_prog;
    cl_kernel m_kernel;
    cl_command_queue m_queue;
};

void do_serial(QVector<float> &x, QVector<float> &y, QVector<float> &z, float a)
{
    for (int i = 0; i < x.size(); i++) {
        z[i] = a * x[i] + y[i];
    }
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    OpenCL ocl;
    ocl.init();
    SaxpyCL saxpy;
    saxpy.init(ocl.get_device());

    float a = 2.0;
    QVector<float> x = {1.0, 2.0, 3.0};
    QVector<float> y = {10.0, 20.0, 30.0};
    QVector<float> act = {0, 0, 0};
    QVector<float> exp = {0, 0, 0};

    do_serial(x, y, exp, a);
    saxpy.compute(a, x, y, act);

    qDebug() << exp;
    qDebug() << act;

    return 0;
}
