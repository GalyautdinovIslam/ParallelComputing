// Написать программу, в которой объявить и присвоить начальные значения элементам двумерного массива d[6][8],
// для инициализации значений использовать генератор случайных чисел. Используя конструкцию директивы
// sections...section определить три секции для выполнения следующих операций:
//
//  —> первая секция выполняет вычисление среднего арифметического значения элементов двумерного массива,
//  —> вторая секция выполняет вычисление минимального и максимального значений элементов двумерного массива,
//  —> третья секция выполняет вычисление количества элементов массива, числовые значения которых кратны 3.
//
//  В каждой секции определить и выдать на экран номер исполняющей нити и результат выполнения вычислений.

#include <iostream>
#include <ctime>
#include <omp.h>

int main() {
    int I = 6;
    int J = 8;

    int d[I][J];
    std::srand(std::time(nullptr));

    for (int i = 0; i < I; i++) {
        for (int j = 0; j < J; j++) {
            d[i][j] = std::rand();
//            std::printf("%5d ", d[i][j]);
        }
//        std::cout << "\n";
    }
//    std::cout << "\n";

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            int c = 0;
            int s = 0;
            for (int i = 0; i < I; i++) {
                for (int j = 0; j < J; j++) {
                    s += d[i][j];
                    c++;
                }
            }

            std::cout << "Section 1: Thread #" + std::to_string(omp_get_thread_num())
            + ". Average = " + std::to_string(s * 1.0 / c) + "\n";
        }
        #pragma omp section
        {
            int min = INT_MAX;
            int max = INT_MIN;

            for (int i = 0; i < I; i++) {
                for (int j = 0; j < J; j++) {
                    if (min > d[i][j]) min = d[i][j];
                    if (max < d[i][j]) max = d[i][j];
                }
            }

            std::cout << "Section 2: Thread #" + std::to_string(omp_get_thread_num())
                         + ". Min = " + std::to_string(min)
                         + " Max = " + std::to_string(max) + "\n";
        }
        #pragma omp section
        {
            int c = 0;
            for (int i = 0; i < I; i++) {
                for (int j = 0; j < J; j++) {
                    if (d[i][j] % 3 == 0) c++;
                }
            }

            std::cout << "Section 3: Thread #" + std::to_string(omp_get_thread_num())
                         + ". Amount = " + std::to_string(c) + "\n";
        }
    }
    return 0;
}
