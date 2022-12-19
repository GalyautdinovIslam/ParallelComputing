// Разработать программу, в которой два процесса многократно обмениваются сообщениями длиной n байт.
// Выполнить эксперименты и оценить зависимость времени выполнения операции от длины сообщения.
// Рассмотреть два варианта обмена сообщениями: PingPong — двунаправленная передача:
// Выполняется передача данных от одного процесса другому с последующим возвратом переданных данных в исходный процесс.
// Время выполнение замеряется после получения обратного ответа на исходном процессе.
// PingPing — однонаправленная передача: Два процесса одновременно выполняют передачу данных друг другу.
// Время выполнения замеряется после получения сообщения каждым процессом.

#include <iostream>
#include <mpi.h>
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>

using random_bytes_engine = std::independent_bits_engine<
        std::random_device, CHAR_BIT, unsigned char>;

void ping_pong(int n, int rank, MPI_Status status,
               int tag, int source, int dest, MPI_Comm comm, MPI_Datatype datatype, MPI_Status *ignore) {
    random_bytes_engine rbe;
    int count;
    double start_time, stop_time, result_time;
    int t = 10000;

    if (rank == source) {
        std::vector<unsigned char> a(n);
        std::generate(begin(a), end(a), std::ref(rbe));

        start_time = MPI_Wtime();
        for (int i = 0; i < t; i++) {
            MPI_Send(&a[0], n, datatype, dest, tag, comm);
            MPI_Recv(&a[0], n, datatype, dest, tag, comm, ignore);
        }
        stop_time = MPI_Wtime();

        result_time = stop_time - start_time;
        std::cout << std::to_string(result_time) + "\n";
    } else if (rank == dest) {
        MPI_Probe(source, tag, comm, &status);
        MPI_Get_count(&status, datatype, &count);

        __attribute__((unused)) unsigned char a[count];
        for (int i = 0; i < t; i++) {
            MPI_Recv(&a[0], count, datatype, source, tag, comm, ignore);
            MPI_Send(&a[0], count, datatype, source, tag, comm);
        }
    }
}

void ping_ping(int n, int rank, int tag, int source, int dest,
               MPI_Comm comm, MPI_Datatype datatype, MPI_Status *ignore) {
    random_bytes_engine rbe;
    double start_time, stop_time, result_time;
    int t = 10000;
    start_time = MPI_Wtime();
    if (rank == source) {
        std::vector<unsigned char> a(n);
        std::generate(begin(a), end(a), std::ref(rbe));
        for (int i = 0; i < t; i++) {
            MPI_Send(&a[0], n, datatype, dest, tag, comm);
        }
    } else if (rank == dest) {
        std::vector<unsigned char> a(n);
        std::generate(begin(a), end(a), std::ref(rbe));
        for (int i = 0; i < t; i++) {
            MPI_Send(&a[0], n, datatype, source, tag, comm);
        }
    }
    if (rank == source) {
        std::vector<unsigned char> a(n);
        for (int i = 0; i < t; i++) {
            MPI_Recv(&a[0], n, datatype, dest, tag, comm, ignore);
        }
    } else if (rank == dest) {
        std::vector<unsigned char> a(n);
        for (int i = 0; i < t; i++) {
            MPI_Recv(&a[0], n, datatype, source, tag, comm, ignore);
        }
    }
    stop_time = MPI_Wtime();
    if (rank == source) {
        result_time = stop_time - start_time;
        std::cout << std::to_string(result_time) + "\n";
    }
}

int main(int argc, char *argv[]) {
    int n = 1000000;

    int rank;
    int size;
    MPI_Status status;
    int tag = 13;
    int source = 0;
    int dest = 1;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_UNSIGNED_CHAR;
    MPI_Status *ignore = MPI_STATUS_IGNORE;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (size != 2) {
        MPI_Finalize();
        return -1;
    }

    ping_pong(n, rank, status, tag, source, dest, comm, datatype, ignore);
//    ping_ping(n, rank, tag, source, dest, comm, datatype, ignore);

    MPI_Finalize();
    return 0;
}
