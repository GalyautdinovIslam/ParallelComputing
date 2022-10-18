// Написать программу, запустить ее на 2-х процессах. На нулевом процессе задать массив a из 10 элементов,
// значения сгенерировать случайным образом. Переслать этот массив целиком первому процессу с помощью функции MPI_Send.
// Распечатать на первом процессе принятый массив.

#include <iostream>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int count = 10;
    int a[count];

    int rank;
    MPI_Status status;
    int source = 0;
    int dest = 1;
    int tag = 13;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_INT;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);

    if (rank == source) {
        std::cout << "Sent: ";
        for (int i = 0; i < count; i++) {
            a[i] = std::rand();
            std::cout << std::to_string(a[i]) + " ";
        }
        std::cout << "\n";
        MPI_Send(a, count, datatype, dest, tag, comm);
    } else if (rank == dest) {
        MPI_Recv(a, count, datatype, source, tag, comm, &status);
        std::cout << "Accepted: ";
        for (int i = 0; i < count; i++) {
            std::cout << std::to_string(a[i]) + " ";
        }
        std::cout << "\n";
    }

    MPI_Finalize();
    return 0;
}
