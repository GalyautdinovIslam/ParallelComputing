// Написать программу, печатающую номер процесса и общее количество процессов в коммуникаторе MPI_COMM_WORLD.

#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    int size;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    std::cout << "Id of process: " + std::to_string(rank) + "; Size: " + std::to_string(size) + ".\n";

    MPI_Finalize();
    return 0;
}
