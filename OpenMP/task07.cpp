// Написать программу, в которой определить две параллельные области, каждая из которых содержит
// итерационную конструкцию for, выполняющую инициализацию элементов одномерных массивов целых чисел a[12],
// b[12] и c[12].Число нитей перед первой областью задать равным 3, перед второй – равным 4. Первая параллельная
// область выполняет инициализацию элементов массивов a и b с использованием статического распределения итераций,
// размер порции итераций выбрать самостоятельно, вторая параллельная область выполняет инициализацию элементов
// массива c по следующему правилу c[i] = a[i] + b[i], с использованием динамического распределения итераций,
// размер порции итераций выбрать самостоятельно. В каждой области определить и выдать на экран количество нитей,
// номер нити и результат выполнения цикла. Убедиться в правильности работы программы.

#include <iostream>
#include <ctime>
#include <omp.h>

// Инициализирую a и b массивы рандомными значениям
// Правильностью программы можно убедиться по выводу:
// 1) Как программа распределила цикл по потокам относительно значений порции итераций
// 2) И по уравнению a[i] + b[i] = c[i]
int main() {
    int TWELVE = 12;
    std::srand(std::time(nullptr));

    int a[TWELVE], b[TWELVE], c[TWELVE];

    if (omp_get_dynamic() != 0) {
        omp_set_dynamic(0);
    }

    #pragma omp parallel for schedule(static, 2) num_threads(3)
    for (int i = 0; i < TWELVE; i++) {
        a[i] = std::rand();
        b[i] = std::rand();
        std::cout << "Thread " + std::to_string(omp_get_thread_num())
                     + " of " + std::to_string(omp_get_num_threads()) + ". "
                     + "Result: a[" + std::to_string(i) + "] = " + std::to_string(a[i]) + ", "
                     + "b[" + std::to_string(i) + "] = " + std::to_string(b[i]) + ". \n";
    }

    std::cout << "\n";

    #pragma omp parallel for schedule(dynamic, 3) num_threads(4)
    for (int i = 0; i < TWELVE; i++) {
        c[i] = a[i] + b[i];
        std::cout << "Thread " + std::to_string(omp_get_thread_num())
                     + " of " + std::to_string(omp_get_num_threads()) + ". "
                     + "Result: c[" + std::to_string(i) + "] = " + std::to_string(c[i]) + ". \n";
    }

    return 0;
}
