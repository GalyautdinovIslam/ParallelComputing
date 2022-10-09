// Написать программу, в которой объявить и присвоить начальные значения массиву целых чисел a[30],
// для инициализации значений использовать генератор случайных чисел. Используя конструкцию omp parallel for и
// omp atomic вычислить количество элементов массива, числовые значения которых кратны 9.
// Количество нитей задать самостоятельно. Результат выдать на экран.

#include <iostream>
#include <ctime>

int main() {
    std::srand(std::time(nullptr));

    int I = 30;
    int N = 4;
    int D = 9;
    int counter = 0;
    int a[I];
    for (int i = 0; i < I; i++) {
        a[i] = std::rand();
//        std::cout << std::to_string(a[i]) + " ";
    }
//    std::cout << "\n";

    #pragma omp parallel for num_threads(N) shared(counter)
    for (int i = 0; i < I; i++) {
        if (a[i] % D == 0) {
            #pragma omp atomic
            counter++;
        }
    }

    std::cout << "The answer is a " + std::to_string(counter) + "\n";
    return 0;
}
