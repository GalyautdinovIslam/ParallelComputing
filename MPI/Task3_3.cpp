// Написать программу умножения матрицы на вектор C = A * b. Распределить матрицу A равномерно вдоль строк, вектор b
// передать всем процессам. После выполнения операции умножения на каждом из процессов необходимо выполнить сбор
// полученных локальных частей результирующего вектора на один процесс (с номером 0). Использовать функцию
// MPI_Bcast для рассылки вектора b, для распределения элементов матрицы использовать функцию MPI_Scatter,
// для сбора локальных частей результирующего вектора в глобальный вектор использовать функцию MPI_Gather.

#include <iostream>
#include <sstream>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 10;

    int n = 5;
    int m = 3;
    int A[n][m], b[m];

    int result[n];
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
        for (int i = 0; i < m; i++) {
            b[i] = std::rand() % MOD - std::rand() % MOD;
            streamB << std::to_string(b[i]) + " ";
            for (int j = 0; j < n; j++) {
                A[j][i] = std::rand() % MOD - std::rand() % MOD;
                streamA << std::to_string(A[j][i]) + " ";
            }
            streamA << "\n";
        }
        std::cout << "Array B: " + streamB.str() + "\n";
        std::cout << "Matrix A:\n" + streamA.str() + "\n";
    }

    int count = n % size == 0 ? n / size : n / size + 1;
    int a[count][m];

    MPI_Bcast(&b[0], m, datatype, root, comm);
    MPI_Scatter(&A[rank * count][0], count * m, datatype, &a[0][0], count * m, datatype, root, comm);

    int vector[count];
    for (int i = 0; i < count && rank * count + i < n; i++) {
        vector[i] = 0;
        for (int j = 0; j < m; j++) {
            vector[i] += a[i][j] * b[j];
        }
    }

    MPI_Gather(&vector[0], count, datatype, &result[rank * count], count, datatype, root, comm);

    if (rank == 0) {
        std::cout << "Result: ";
        for (int i = 0; i < n; i++) {
            std::cout << std::to_string(result[i]) + " ";
        }
        std::cout << "\n";
    }

    MPI_Finalize();
    return 0;
}