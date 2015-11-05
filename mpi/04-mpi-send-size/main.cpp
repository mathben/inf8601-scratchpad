#include <QDebug>
#include <mpi.h>

int main(int argc, char **argv)
{
    int numprocs, rank;
    MPI_Status status;

    int max = (1 << 23);
    QVector<int> send(max);
    QVector<int> recv(max);

    qFill(send, 42);
    qFill(recv, 0);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (numprocs != 2) {
        printf("Error: number of processors must be equal to 2\n");
        MPI_Finalize();
        return -1;
    }

    if (rank == 0) {
        for (int i = 1; i < max; i = i * 2) {
            qDebug() << "sending" << i;
            MPI_Send(send.data(), i, MPI_INT, 1, 1, MPI_COMM_WORLD);
        }
    } else {
        for (int i = 1; i < max; i = i * 2) {
            MPI_Recv(recv.data(), i, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        }
    }
    qDebug() << "rank" << rank << "done";

    MPI_Finalize();
    return 0;
}
