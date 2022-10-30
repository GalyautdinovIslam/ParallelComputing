// В программе реализовать поэлементное умножение матриц: C = A ° B
// (Cij = Aij * Bij)

#include <iostream>
#include <sstream>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 10;

    int n = 5;

    int rank;
    int size;
    int count;
    int forLast;
    MPI_Status status;
    int source = 0;
    int tagA = 13;
    int tagB = 31;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_INT;
    MPI_Status *ignore = MPI_STATUS_IGNORE;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == source) {
        int a[n * n], b[n * n], c[n * n];
        if ((n * n) % (size - 1) == 0) count = n * n / (size - 1);
        else count = n * n / (size - 1) + 1;
        forLast = n * n - count * (size - 2);
        std::stringstream streamA;
        std::stringstream streamB;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i * n + j] = std::rand() % MOD;
                streamA << std::to_string(a[i * n + j]) + " ";
                b[i * n + j] = std::rand() % MOD;
                streamB << std::to_string(b[i * n + j]) + " ";
            }
            streamA << "\n";
            streamB << "\n";
        }

        std::cout << "Matrix A:\n" + streamA.str() + "\n";
        std::cout << "Matrix B:\n" + streamB.str() + "\n";

        for (int dest = 1; dest < size; dest++) {
            int amount;
            if (size - dest != 1) amount = count;
            else amount = forLast;
            MPI_Send(&a[(dest - 1) * count], amount, datatype, dest, tagA + dest, comm);
            MPI_Send(&b[(dest - 1) * count], amount, datatype, dest, tagB + dest, comm);
        }

        for (int dest = 1; dest < size; dest++) {
            int amount;
            if (size - dest != 1) amount = count;
            else amount = forLast;
            MPI_Recv(&c[(dest - 1) * count], amount, datatype, dest, tagA + dest, comm, ignore);
        }

        std::stringstream streamC;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                streamC << std::to_string(c[i * n + j]) + " ";
            }
            streamC << "\n";
        }

        std::cout << "Matrix C:\n" + streamC.str() + "\n";
    } else {
        MPI_Probe(source, tagA + rank, comm, &status);
        MPI_Get_count(&status, datatype, &count);

        int a[count], b[count], c[count];

        MPI_Recv(&a[0], count, datatype, source, tagA + rank, comm, ignore);
        MPI_Recv(&b[0], count, datatype, source, tagB + rank, comm, ignore);

        for (int i = 0; i < count; i++) {
            c[i] = a[i] * b[i];
        }

        MPI_Send(&c[0], count, datatype, source, tagA + rank, comm);
    }

    MPI_Finalize();
    return 0;
}