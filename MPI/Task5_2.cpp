// Создать тип данных с помощью функции MPI_Type_create_struct и выполнить рассылку строк матрицы A[8][8]
// на 4 процесса в матрицу D[2][8] по следующей схеме:
// — на нулевой процесс строки 0 и 4,
// — на первый процесс строки 1 и 5,
// — на второй процесс строки 2 и 6,
// — на третий процесс строки 3 и 7.

#include <iostream>
#include <sstream>
#include <ctime>
#include <mpi.h>

struct Dummy {
    int a[8];
    int b[8];
} ;


int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 10;

    int n = 8;

    int rank;
    int size;
    int source = 0;
    int tag = 13;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Status *ignore = MPI_STATUS_IGNORE;

    int array_of_blocklengths[2] = {n, n};
    MPI_Datatype array_of_types[2] = {MPI_INT, MPI_INT};
    MPI_Aint array_of_displacements[2];
    MPI_Datatype vectors;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    array_of_displacements[0] = offsetof(struct Dummy, a);
    array_of_displacements[1] = offsetof(struct Dummy, b);
    MPI_Type_create_struct(2, array_of_blocklengths, array_of_displacements, array_of_types, &vectors);
    MPI_Type_commit(&vectors);

    if (size != 5) {
        MPI_Finalize();
        return -1;
    }

    if (rank == source) {
        struct Dummy dummy = *new Dummy();
        int a[n][n];

        std::stringstream streamA;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = std::rand() % MOD;
                streamA << std::to_string(a[i][j]) + " ";
            }
            streamA << "\n";
        }
        std::cout << "Matrix A:\n" + streamA.str() + "\n";

        for (int i = 1; i < 5; i++) {
            for (int j = 0; j < n; j++) {
                dummy.a[j] = a[i - 1][j];
                dummy.b[j] = a[i + 3][j];
            }
            MPI_Send(&dummy, 1, vectors, i, tag + i, comm);
        }
    } else {
        struct Dummy dummy = *new Dummy();
        int d[2][n];

        MPI_Recv(&dummy, 1, vectors, source, tag + rank, comm, ignore);
        std::stringstream streamA;
        for (int i = 0; i < n; i++) {
            d[0][i] = dummy.a[i];
            d[1][i] = dummy.b[i];
            streamA << std::to_string(d[0][i]) + " ";
        }
        streamA << "\n";
        for (int i = 0; i < n; i++) {
            streamA << std::to_string(d[1][i]) + " ";
        }
        streamA << "\n";
        std::cout << "Matrix D_" + std::to_string(rank) +":\n" + streamA.str() + "\n";
    }

    MPI_Finalize();
    return 0;
}