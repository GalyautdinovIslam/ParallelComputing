// В программе реализовать транспонирование матрицы: B = A^t
// (^ — не возведение в степень, а указатель на верхний индекс)

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
    int tagC = 31;
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
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    a[i][j] = std::rand() % MOD;
                    streamA << std::to_string(a[i][j]) + " ";
                    c[i][j] = a[i][j];
                }
                streamA << "\n";
            }

            std::cout << "Matrix A:\n" + streamA.str() + "\n";

            for (int i = 0, dest = 1; i < n; i += block, dest++) {
                MPI_Send(&(c[0][0]), n * n, datatype, dest, tagC + dest, comm);
                int count = block < n - i ? block : n - i;
                MPI_Send(&(a[i][0]), count * n, datatype, dest, tagA + dest, comm);
            }

            for (int i = 0, dest = 1; i < n; i += block, dest++) {
                int count = block < n - i ? block : n - i;
                MPI_Recv(&(b[i][0]), count * n, datatype, dest, tagA + dest, comm, ignore);
            }

            std::stringstream streamB;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    streamB << std::to_string(b[i][j]) + " ";
                }
                streamB << "\n";
            }

            std::cout << "Matrix B:\n" + streamB.str() + "\n";
        } else {
            int cCount;
            int aCount;
            MPI_Probe(source, tagC + rank, comm, &status);
            MPI_Get_count(&status, datatype, &cCount);
            MPI_Probe(source, tagA + rank, comm, &status);
            MPI_Get_count(&status, datatype, &aCount);

            int m = (int) sqrt(cCount);
            int count = aCount / m;
            int a[count][n], c[n][n];

            MPI_Recv(&(c[0][0]), cCount, datatype, source, tagC + rank, comm, ignore);
            MPI_Recv(&(a[0][0]), aCount, datatype, source, tagA + rank, comm, ignore);

            for (int i = 0; i < count; i++) {
                for (int j = 0; j < m; j++) {
                    a[i][j] = c[j][i + count * (rank - 1)];
                }
            }
            MPI_Send(&(a[0][0]), aCount, datatype, source, tagA + rank, comm);
        }
    }

    MPI_Finalize();
    return 0;
}