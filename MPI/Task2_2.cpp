// Написать программу, которая меняет местами элементы векторов Xi <-> Yi. Векторы X, Y задаются на нулевом процессе
// и равными блоками пересылаются остальным процессам. Пересылка данных осуществляется функцией MPI_Send.
// Каждый процесс отправляет на нулевой процесс подсчитанные элементы.

#include <iostream>
#include <sstream>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 100;

    int n = 13;

    int rank;
    int size;
    int count;
    int forLast;
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
        int x[n], y[n];
        if (n % (size - 1) == 0) count = n / (size - 1);
        else count = n / (size - 1) + 1;
        forLast = n - count * (size - 2);
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
            int amount;
            if (size - dest != 1) amount = count;
            else amount = forLast;
            MPI_Send(&x[(dest - 1) * count], amount, datatype, dest, tagX + dest, comm);
            MPI_Send(&y[(dest - 1) * count], amount, datatype, dest, tagY + dest, comm);
        }

        for (int dest = 1; dest < size; dest++) {
            int amount;
            if (size - dest != 1) amount = count;
            else amount = forLast;
            MPI_Recv(&x[(dest - 1) * count], amount, datatype, dest, tagX + dest, comm, ignore);
            MPI_Recv(&y[(dest - 1) * count], amount, datatype, dest, tagY + dest, comm, ignore);
        }

        streamX.str(std::string());
        streamY.str(std::string());

        for (int i = 0; i < n; i++) {
            streamX << std::to_string(x[i]) + " ";
            streamY << std::to_string(y[i]) + " ";
        }

        std::cout << "Array X: " + streamX.str() + "\n";
        std::cout << "Array Y: " + streamY.str() + "\n";
    } else {
        MPI_Probe(source, tagX + rank, comm, &status);
        MPI_Get_count(&status, datatype, &count);

        int x[count], y[count];

        MPI_Recv(&x[0], count, datatype, source, tagX + rank, comm, ignore);
        MPI_Recv(&y[0], count, datatype, source, tagY + rank, comm, ignore);

        MPI_Send(&y[0], count, datatype, source, tagX + rank, comm);
        MPI_Send(&x[0], count, datatype, source, tagY + rank, comm);
    }

    MPI_Finalize();
    return 0;
}