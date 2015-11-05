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
        /*
         * En premier lieu, il faut obtenir les identifiants des
         * plateformes disponibles. Avant, il faut allouer la mémoire
         * requise pour contenir ces identifiants, donc il faut savoir
         * le nombre de plateformes disponibles.
         */
        cl_uint nrPlatforms = 0;
        clGetPlatformIDs(0, nullptr, &nrPlatforms);

        if (nrPlatforms == 0)
            throw std::runtime_error("No OpenCL platform");

        /*
         * Maintenant que le vecteur est de la bonne taille, on obtient
         * les identifiants eux-mêmes.
         */
        m_platforms.resize(nrPlatforms);
        clGetPlatformIDs(nrPlatforms, m_platforms.data(), nullptr);

        /*
         * Inspection des périphériques (device) pour chaque plateforme.
         * Ceci se fait aussi en deux étapes: on obtient le nombre de device
         * pour la plateforme, on alloue le vecteur et finalement on obtient
         * les identifiants de périphérique.
         */
        for(const auto id: m_platforms) {
            cl_uint nDev = 0;
            clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, 0, nullptr, &nDev);
            QVector<cl_device_id> deviceIDs(nDev);
            clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, nDev, deviceIDs.data(), nullptr);
            for (const auto dev: deviceIDs) {
                m_devices.insert(dev, id);
            }
        }
        if (m_devices.size() == 0)
            throw std::runtime_error("No OpenCL device");

        qDebug() << "platforms" << m_platforms;
        qDebug() << "devices" << m_devices;

    }

    /* Par simplicité, on retourne le premier device */
    cl_device_id get_device() {
        return m_devices.keys().first();
    }

    QVector<cl_platform_id> m_platforms;
    QMap<cl_device_id, cl_platform_id> m_devices;
};

#define MULTI_LINE_STRING(...) #__VA_ARGS__
class SaxpyCL {

    /*
     * La chaine de caractère du kernel. Cette chaine représente
     * le code qui est compilé pour le périphérique OpenCL. On utilise
     * une macro pour éviter les guillemets, mais dans le laboratoire,
     * on ajoute la chaine au fichier objet après avoir vérifié qu'il
     * compile (en utilisant openclcc).
     *
     * __kernel : indique une fonction que l'on peut appeler
     * __global : indique un tampon créé avec clCreateBuffer()
     */
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
        /* Création du contexte (regroupe toutes les opérations reliées) */
        m_ctx       = clCreateContext(nullptr, 1, &dev, nullptr, nullptr, &ret);

        /* Création du programme (mais ne le compile pas) */
        m_prog      = clCreateProgramWithSource(m_ctx, 1, &kernel_text, nullptr, nullptr);

        /* Compile le programme */
        ret         = clBuildProgram(m_prog, 0, nullptr, "", nullptr, nullptr);

        /* obtient une référence du kernel (on doit spécifier un nom de fonction annotée avec __kernel) */
        m_kernel    = clCreateKernel(m_prog, "SAXPY", &ret);

        /* Création de la file d'attente de commande (les commandes sont exécutées dans l'ordre FIFO) */
        m_queue     = clCreateCommandQueue(m_ctx, dev, 0, &ret);
    }

    void compute(float a, QVector<float> &x, QVector<float> &y, QVector<float> &z) {
        cl_int ret;

        /* la taille en octets des tampons, requis pour allouer les tampons sur le périphérique */
        size_t size = x.size() * sizeof(float);

        /*
         * work_size est un tableau des tailles pour chaque dimensions. Ici, les
         * vecteurs sont des donnée 1D.
         *
         * Si on travaille en 2D, alors le tableau a deux entiers: {x, y}
         * Le kernel est appelé pour chaque élément de dimension, et l'index
         * est retourné par get_global_id(dimension). get_global_id(0) retourne
         * l'indexe x et get_global_id(1) retourne l'indexe y.
         */
        size_t work_size[] = { size };

        /*
         * Création des tampons sur la carte. Les opérations CL_MEM_READ/CL_MEM_WRITE
         * sont du point de vue du kernel. Ici, le kernel lit depuis x et y, mais
         * écrit dans z.
         */
        cl_mem cl_x = clCreateBuffer(m_ctx, CL_MEM_READ_ONLY, size, nullptr, &ret);
        cl_mem cl_y = clCreateBuffer(m_ctx, CL_MEM_READ_ONLY, size, nullptr, &ret);
        cl_mem cl_z = clCreateBuffer(m_ctx, CL_MEM_WRITE_ONLY, size, nullptr, &ret);

        /*
         * Copie des données de l'ordinateur vers la carte. Les opérations Read/Write sont
         * du point de vue de l'ordinateur hôte. On écrit vers la carte les données des
         * vecteurs x et y.
         */
        clEnqueueWriteBuffer(m_queue, cl_x, CL_FALSE, 0, size, x.data(), 0, nullptr, nullptr);
        clEnqueueWriteBuffer(m_queue, cl_y, CL_FALSE, 0, size, y.data(), 0, nullptr, nullptr);

        /* Définition des arguments en ordre */
        clSetKernelArg(m_kernel, 0, sizeof(cl_mem), &cl_x);
        clSetKernelArg(m_kernel, 1, sizeof(cl_mem), &cl_y);
        clSetKernelArg(m_kernel, 2, sizeof(cl_mem), &cl_z);
        clSetKernelArg(m_kernel, 3, sizeof(float), &a);

        /* Lancement du calcul */
        clEnqueueNDRangeKernel(m_queue, m_kernel, 1, nullptr, work_size, nullptr, 0, nullptr, nullptr);

        /* Récupération du résultat (copie de la carte vers l'hôte) */
        clEnqueueReadBuffer(m_queue, cl_z, CL_TRUE, 0, size, z.data(), 0, nullptr, nullptr);

        /* Libération des tampons sur la carte */
        clReleaseMemObject(cl_x);
        clReleaseMemObject(cl_y);
        clReleaseMemObject(cl_z);
    }

    void close() {
        clFinish(m_queue);
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
