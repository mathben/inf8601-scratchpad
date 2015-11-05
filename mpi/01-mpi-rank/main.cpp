#include <QDebug>

#include <mpi.h>

int main(int argc, char *argv[])
{
    /* C++ */
    MPI::Init(argc, argv);

    int size = MPI::COMM_WORLD.Get_size();
    int rank = MPI::COMM_WORLD.Get_rank();

    qDebug() << "C++ size=" << size << "rank=" << rank;

    MPI::COMM_WORLD.Barrier();

    /* C */
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    qDebug() << "C   size=" << size << "rank=" << rank;

    MPI_Barrier(MPI_COMM_WORLD);

    MPI::Finalize();
    return 0;
}
