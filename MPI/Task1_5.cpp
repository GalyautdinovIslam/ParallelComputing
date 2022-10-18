// Написать программу, вычисляющую элементы вектора Z по формуле Zi = A * Xi + B * Yi. Векторы X, Y задаются на
// нулевом процессе и равными блоками пересылаются остальным процессам, A и B — заданные числа.
// Пересылка данных осуществляется функцией MPI_Send. Все процессы по формуле вычисляют свои элементы массива Z.
// Каждый процесс отправляет на нулевой процесс подсчитанные элементы.

#include <iostream>
#include <sstream>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 10;

    int n = 12;
    int A = 2;
    int B = -1;

    int rank;
    int size;
    int count;
    MPI_Status status;
    int source = 0;
    int tagX = 13;
    int tagY = 31;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_INT;
    MPI_Status *ignore = MPI_STATUS_IGNORE;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == source) {
        int x[n], y[n], z[n];
        count = n / (size - 1) + 1;
        std::stringstream streamX;
        std::stringstream streamY;
        for (int i = 0; i < n; i++) {
            x[i] = std::rand() % MOD;
            streamX << std::to_string(x[i]) + " ";
            y[i] = std::rand() % MOD;
            streamY << std::to_string(y[i]) + " ";
        }

        std::cout << "Array X: " + streamX.str() + "\n";
        std::cout << "Array Y: " + streamY.str() + "\n";

        for (int dest = 1; dest < size; dest++) {
            MPI_Send(&x[(dest - 1) * count], count, datatype, dest, tagX + dest, comm);
            MPI_Send(&y[(dest - 1) * count], count, datatype, dest, tagY + dest, comm);
        }

        for (int dest = 1; dest < size; dest++) {
            MPI_Recv(&z[(dest - 1) * count], count, datatype, dest, tagX + dest, comm, ignore);
        }

        std::stringstream streamZ;
        for (int i = 0; i < n; i++) {
            streamZ << std::to_string(z[i]) + " ";
        }
        std::cout << "Array Z: " + streamZ.str() + "\n";
    } else {
        MPI_Probe(source, tagX + rank, comm, &status);
        MPI_Get_count(&status, datatype, &count);

        int x[count], y[count], z[count];

        MPI_Recv(&x[0], count, datatype, source, tagX + rank, comm, ignore);
        MPI_Recv(&y[0], count, datatype, source, tagY + rank, comm, ignore);

        for (int i = 0; i < count; i++) {
            z[i] = A * x[i] + B * y[i];
        }
        MPI_Send(&z[0], count, datatype, source, tagX + rank, comm);
    }

    MPI_Finalize();
    return 0;
}
