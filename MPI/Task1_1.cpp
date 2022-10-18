// Написать mpi программу, печатающую "Hello, World!". Запустить программу на 4 процессах.

#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    std::cout << "Hello, World!\n";

    MPI_Finalize();
    return 0;
}
