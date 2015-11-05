#include <QDebug>
#include <mpi.h>

int main(int argc, char **argv)
{
    int numprocs, rank;
    MPI_Status status;
    MPI_Comm comm1d;

    int max = (1 << 20);
    QVector<int> send(max);
    QVector<int> recv(max);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int dims[] = { numprocs };
    int periods[] = { 1 };
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, 0, &comm1d);


    int next;
    int prev;
    MPI_Cart_shift(comm1d, 0, 1, &prev, &next);
    qDebug() << "prev" << prev << "rank" << rank << "next" << next;
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);

    qFill(send, rank);

    for (int i = 1; i < max; i = i * 2) {
        qFill(recv, 0);
        qDebug() << "irecv exchange" << i << rank << prev << "->" << next;
        //MPI_Sendrecv(send.data(), i, MPI_INT, next, 0, recv.data(), i, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);

        MPI_Request req_recv;
        MPI_Request req_send;
        MPI_Isend(send.data(), i, MPI_INT, next, 0, MPI_COMM_WORLD, &req_send);
        MPI_Irecv(recv.data(), i, MPI_INT, prev, 0, MPI_COMM_WORLD, &req_recv);
        MPI_Wait(&req_recv, &status);
        MPI_Wait(&req_send, &status);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    qDebug() << "rank" << rank << "done";

    return 0;
}
