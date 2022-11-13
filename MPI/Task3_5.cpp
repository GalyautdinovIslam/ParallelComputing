// Написать программу вычисления поэлементного умножения матриц С = A ° B
// (Cij = Aij * Bij).
// Использовать функции MPI_Scatter для распределения элементов матриц A и B
// и MPI_Gather для сбора вычисленных данных в матрицу C.

#include <iostream>
#include <sstream>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 10;

    int n = 5;
    int m = 3;
    int A[n][m], B[n][m];

    int C[n][m];
    int rank;
    int size;
    int root = 0;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_INT;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == 0) {
        std::stringstream streamA;
        std::stringstream streamB;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                A[i][j] = std::rand() % MOD - std::rand() % MOD;
                streamA << std::to_string(A[i][j]) + " ";
                B[i][j] = std::rand() % MOD - std::rand() % MOD;
                streamB << std::to_string(B[i][j]) + " ";
            }
            streamA << "\n";
            streamB << "\n";
        }
        std::cout << "Matrix A:\n" + streamA.str() + "\n";
        std::cout << "Matrix B:\n" + streamB.str() + "\n";
    }

    int count = n % size == 0 ? n / size : n / size + 1;
    int a[count][m], b[count][m];

    MPI_Scatter(&A[rank * count][0], count * m, datatype, &a[0][0], count * m, datatype, root, comm);
    MPI_Scatter(&B[rank * count][0], count * m, datatype, &b[0][0], count * m, datatype, root, comm);

    int c[count][m];
    for (int i = 0; i < count && rank * count + i < n; i++) {
        for (int j = 0; j < m; j++) {
            c[i][j] = a[i][j] * b[i][j];
        }
    }

    MPI_Gather(&c[0][0], count * m, datatype, &C[rank * count][0], count * m, datatype, root, comm);

    if (rank == 0) {
        std::cout << "Matrix C:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                std::cout << std::to_string(C[i][j]) + " ";
            }
            std::cout << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}