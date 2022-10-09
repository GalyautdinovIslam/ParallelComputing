// Используя возможности OpenMP, написать программу умножения матрицы на вектор.
// Сравнить время выполнения последовательной и параллельных программ.

#include <iostream>
#include <ctime>
#include <windows.h>
#include <omp.h>

// Заполняем наши массивы
void initArrays(int *a, int *b, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = std::rand() % 100;
//            printf("%2d ", a[i * n + j]);
        }
//        std::cout << "\n";
    }
//    std::cout << "\n";
    for (int j = 0; j < n; j++) {
        b[j] = std::rand() % 100;
//        std::cout << std::to_string(b[j]) + " ";
    }
//    std::cout << "\n\n";
}

// Чистим массив (по сути лишнее, но пусть будет, время самих алгоритмов не портит)
void free(int *c, int m) {
    for (int i = 0; i < m; i++) {
        c[i] = 0;
    }
}

// Печатаем наш массив
void printArray(int *c, int m) {
    for (int i = 0; i < m; i++) {
        std::cout << std::to_string(c[i]) + " ";
    }
    std::cout << "\n\n";
}

// Последовательный алгоритм
void runNoParallel(const int *a, const int *b, int *c, int m, int n) {
    for (int i = 0; i < m; i++) {
        c[i] = 0;
        for (int j = 0; j < n; j++) {
            c[i] += a[i * n + j] * b[j];
        }
    }
}

// Параллельный алгоритм
void runParallel(const int *a, const int *b, int *c, int m, int n) {
    #pragma omp parallel
    {
        int nThreads = omp_get_num_threads();
        int threadId = omp_get_thread_num();
        int itemsPerThread = m / nThreads;
        int l = threadId * itemsPerThread;
        int r = (threadId == nThreads - 1) ? (m) : (l + itemsPerThread);
        for (int i = l; i < r; i++) {
            c[i] = 0;
            for (int j = 0; j < n; j++) {
                c[i] += a[i * n + j] * b[j];
            }
        }
    }
}

const int m = 15000, n = 15000;
int a[m * n], b[n], c[m];

int main() {
    std::srand(std::time(nullptr));
    DWORD t;
    DWORD noParallel;
    DWORD parallel;

    initArrays(a, b, m, n);

    t = GetTickCount();
    runNoParallel(a, b, c, m, n);
    noParallel = GetTickCount() - t;
    std::cout << "Result time (NO PARALLEL) = " << noParallel << " ms.\n";
//    printArray(c, m);

    free(c, m);

    t = GetTickCount();
    runParallel(a, b, c, m, n);
    parallel = GetTickCount() - t;
    std::cout << "Result time (PARALLEL) = " << parallel << " ms.\n";
//    printArray(c, m);

    if (noParallel < parallel)
        std::cout << "No parallel algorithm was faster.";
    else if (noParallel > parallel)
        std::cout << "Parallel algorithm was faster.";
    else
        std::cout << "The results are the same.";

    return 0;
}
