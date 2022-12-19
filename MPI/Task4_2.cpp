// Написать программу и запустить ее на p(= 2, 3, 4, 6, 8) процессах. На нулевом процессе объявить
// и задать массив из 12 элементов. С помощью функции MPI_Send разослать блоки массива на остальные процессы.
// Размер блока массива (12/p)+1. В результате на нулевом процессе должны быть элементы массива с 0 до 12/p,
// на первом процессе с 12/p+1 до 2×(12/p), на 3м процессе с 2×(12/p)+1  до  3×(12/p)  и т.д.
// Вывести элементы массива на экран на каждом процессе. Обратите внимание, что не на все процессы хватит элементов.

#include <iostream>
#include <sstream>
#include <ctime>
#include <cmath>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 10;

    int n = 12;

    int rank;
    int size;
    MPI_Status status;
    int source = 0;
    int tag = 13;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_INT;
    MPI_Status *ignore = MPI_STATUS_IGNORE;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int count = n / size + 1;
    if (rank == 0) {
        int a[n];
        std::stringstream streamA;
        for (int i = 0; i < n; i++) {
            a[i] = std::rand() % MOD - std::rand() % MOD;
            streamA << std::to_string(a[i]) + " ";
        }
        std::cout << "Array: " + streamA.str() + "\n";

        for (int i = 1; i < size; i++) {
            int toSend = std::min(std::max(0, n - i * count), count);
            MPI_Send(&a[i * count], toSend, datatype, i, tag + i, comm);
        }

        std::stringstream streamP;
        streamP << "From process #" + std::to_string(rank) + ": ";
        for (int i = 0; i < count; i++) {
            streamP << std::to_string(a[i]) + " ";
        }
        streamP << "\n";

        std::cout << streamP.str();

    } else {

        int sent;
        MPI_Probe(source, tag + rank, comm, &status);
        MPI_Get_count(&status, datatype, &sent);

        int c[sent];
        MPI_Recv(&c[0], sent, datatype, source, tag + rank, comm, ignore);

        std::stringstream streamP;
        streamP << "From process #" + std::to_string(rank) + ": ";
        for (int i = 0; i < sent; i++) {
            streamP << std::to_string(c[i]) + " ";
        }
        streamP << "\n";

        std::cout << streamP.str();
    }

    MPI_Finalize();
    return 0;
}