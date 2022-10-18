// Используя блокирующую операцию передачи сообщений (MPI_Send и MPI_Recv) выполнить пересылку данных
// одномерного массива, из процесса с номером 1 на остальные процессы группы. На процессах получателях до выставления
// функции приема (MPI_Recv) определить количество принятых данных (MPI_Probe). Выделить память под размер
// приемного буфера, после чего вызвать функцию MPI_Recv. Полученное сообщение выдать на экран.

#include <iostream>
#include <sstream>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    int n = 10;
    int a[n];

    int rank;
    int size;
    int count;
    MPI_Status status;
    int source = 1;
    int tag = 13;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_INT;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == source) {
        for (int i = 0; i < n; i++) {
            a[i] = std::rand();
        }
        for (int dest = 0; dest < size; dest++) {
            if (dest != source) MPI_Send(&a, n, datatype, dest, tag + dest, comm);
        }
    } else {
        MPI_Probe(source, tag + rank, comm, &status);
        MPI_Get_count(&status, datatype, &count);
        MPI_Recv(&a, count, datatype, source, tag + rank, comm, &status);
        std::stringstream stream;
        stream << "From " + std::to_string(rank) + " process:\n";
        for (int i = 0; i < count; i++) {
            stream << std::to_string(a[i]) + " ";
        }
        stream << "\n";
        std::cout << stream.str();
    }

    MPI_Finalize();
    return 0;
}
