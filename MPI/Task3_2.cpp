// Написать программу вычисления скалярного произведения двух векторов (X, Y) = Сумма по i (Xi * Yi).
// Для распределения элементов вектора X по процессам использовать функцию MPI_Scatter.
// Для получения итоговой суммы на нулевом процессе использовать функцию MPI_Reduce с операцией MPI_Sum.

#include <iostream>
#include <sstream>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 10;

    int n = 10;
    int x[n], y[n];

    int result;
    int rank;
    int size;
    int root = 0;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_INT;
    MPI_Op op = MPI_SUM;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == 0) {
        std::stringstream streamX;
        std::stringstream streamY;
        for (int i = 0; i < n; i++) {
            x[i] = std::rand() % MOD - std::rand() % MOD;
            streamX << std::to_string(x[i]) + " ";
            y[i] = std::rand() % MOD - std::rand() % MOD;
            streamY << std::to_string(y[i]) + " ";
        }
        std::cout << "Array X: " + streamX.str() + "\n";
        std::cout << "Array Y: " + streamY.str() + "\n";
    }

    int count = n % size == 0 ? n / size : n / size + 1;
    int a[count], b[count];

    MPI_Scatter(&x[rank * count], count, datatype, &a[0], count, datatype, root, comm);
    MPI_Scatter(&y[rank * count], count, datatype, &b[0], count, datatype, root, comm);

    int sum = 0;
    for (int i = 0; i < count && rank * count + i < n; i++) {
        sum += a[i] * b[i];
    }

    MPI_Reduce(&sum, &result, 1, datatype, op, root, comm);
    if (rank == 0) {
        std::cout << "Result: " + std::to_string(result) + "\n";
    }

    MPI_Finalize();
    return 0;
}