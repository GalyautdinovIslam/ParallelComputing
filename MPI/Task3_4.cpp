// Написать программу вычисления нормы матрицы A по формуле ||A|| = max{Сумма по j (|Aij|) | j = 1, 2, ..., m},
// где A — m × n матрица. Для распределения элементов матрицы А по процессам использовать функцию MPI_Scatter.
// Для получения итогового значения использовать функцию MPI_Reduce с операцией MPI_MAX.

#include <iostream>
#include <sstream>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 10;

    int n = 5;
    int m = 3;
    int A[n][m];

    int result;
    int rank;
    int size;
    int root = 0;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_INT;
    MPI_Op op = MPI_MAX;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == 0) {
        std::stringstream streamA;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                A[i][j] = std::rand() % MOD - std::rand() % MOD;
                streamA << std::to_string(A[i][j]) + " ";
            }
            streamA << "\n";
        }
        std::cout << "Matrix A:\n" + streamA.str() + "\n";
    }

    int count = n % size == 0 ? n / size : n / size + 1;
    int a[count][m];

    MPI_Scatter(&A[rank * count][0], count * m, datatype, &a[0][0], count * m, datatype, root, comm);

    int sum;
    int max = INT_MIN;
    for (int i = 0; i < count && rank * count + i < n; i++) {
        sum = 0;
        for (int j = 0; j < m; j++) {
            sum += a[i][j];
        }
        max = sum > max ? sum : max;
    }

    MPI_Reduce(&max, &result, 1, datatype, op, root, comm);

    if (rank == 0) {
        std::cout << "Result: " + std::to_string(result) + "\n";
    }

    MPI_Finalize();
    return 0;
}