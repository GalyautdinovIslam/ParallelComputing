// На процессе с номером 0 объявить и заполнить значениями  матрицу  a[8][8]. Создать новый тип данных
// для рассылки нечетных строк матрицы A в матрицу B[4][8], и  для рассылки четных строк матрицы в матрицу C[4][8],
// используя функцию MPI_Type_vector.

#include <iostream>
#include <sstream>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 10;

    int n = 8;

    int rank;
    int size;
    int source = 0;
    int dest_1 = 1;
    int dest_2 = 2;
    int tag = 13;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_INT;
    MPI_Status *ignore = MPI_STATUS_IGNORE;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (size != 3) {
        MPI_Finalize();
        return -1;
    }

    if (rank == source) {
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


        MPI_Datatype vector;
        MPI_Type_contiguous(n, datatype, &vector);
        MPI_Type_commit(&vector);
        MPI_Datatype vectors;
        MPI_Type_vector(n / 2, 1, 2, vector, &vectors);
        MPI_Type_commit(&vectors);

        MPI_Send(&a[1][0], 1, vectors, dest_1, tag + dest_1, comm);
        MPI_Send(&a[0][0], 1, vectors, dest_2, tag + dest_2, comm);
    } else {
        int a[n / 2][n];

        MPI_Recv(&a[0][0], n * n / 2, datatype, source, tag + rank, comm, ignore);
        std::stringstream streamA;
        for (int i = 0; i < n / 2; i++) {
            for (int j = 0; j < n; j++) {
                streamA << std::to_string(a[i][j]) + " ";
            }
            streamA << "\n";
        }
        std::cout << "Matrix " + std::string(1, static_cast<char>('A' + rank)) +":\n" + streamA.str() + "\n";
    }

    MPI_Finalize();
    return 0;
}