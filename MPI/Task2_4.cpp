// В программе реализовать умножение матриц: C = A * B

#include <iostream>
#include <sstream>
#include <ctime>
#include <cmath>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int MOD = 10;

    int n = 3;

    int rank;
    int size;
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

    size -= 1;
    size = size < n ? size : n;
    int block = n / size + bool(n % size != 0);
    if (rank * block - block < n) {
        if (rank == source) {
            int a[n][n], b[n][n], c[n][n];

            std::stringstream streamA;
            std::stringstream streamB;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    a[i][j] = std::rand() % MOD;
                    streamA << std::to_string(a[i][j]) + " ";
                    b[i][j] = std::rand() % MOD;
                    streamB << std::to_string(b[i][j]) + " ";
                }
                streamA << "\n";
                streamB << "\n";
            }

            std::cout << "Matrix A:\n" + streamA.str() + "\n";
            std::cout << "Matrix B:\n" + streamB.str() + "\n";

            for (int i = 0, dest = 1; i < n; i += block, dest++) {
                MPI_Send(&(b[0][0]), n * n, datatype, dest, tagB + dest, comm);
                int count = block < n - i ? block : n - i;
                MPI_Send(&(a[i][0]), count * n, datatype, dest, tagA + dest, comm);
            }

            for (int i = 0, dest = 1; i < n; i += block, dest++) {
                int count = block < n - i ? block : n - i;
                MPI_Recv(&(c[i][0]), count * n, datatype, dest, tagA + dest, comm, ignore);
            }

            std::stringstream streamC;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    streamC << std::to_string(c[i][j]) + " ";
                }
                streamC << "\n";
            }

            std::cout << "Matrix C:\n" + streamC.str() + "\n";
        } else {
            int bCount;
            int aCount;
            MPI_Probe(source, tagB + rank, comm, &status);
            MPI_Get_count(&status, datatype, &bCount);
            MPI_Probe(source, tagA + rank, comm, &status);
            MPI_Get_count(&status, datatype, &aCount);

            int m = (int) sqrt(bCount);
            int count = aCount / m;
            int a[count][n], b[n][n], c[count][n];

            MPI_Recv(&(b[0][0]), bCount, datatype, source, tagB + rank, comm, ignore);
            MPI_Recv(&(a[0][0]), aCount, datatype, source, tagA + rank, comm, ignore);

            for (int i = 0; i < count; i++) {
                for (int j = 0; j < m; j++) {
                    c[i][j] = 0;
                    for (int k = 0; k < m; k++) {
                        c[i][j] += a[i][k] * b[k][j];
                    }
                }
            }
            MPI_Send(&(c[0][0]), aCount, datatype, source, tagA + rank, comm);
        }
    }

    MPI_Finalize();
    return 0;
}