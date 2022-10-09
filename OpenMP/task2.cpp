// Написать программу, в которой определить две параллельные области, выполнение которых
// зависит от условного оператора #pragma omp parallel if(...), если заданное значение числа нитей больше 1,
// параллельная область выполняется, иначе не выполняется. Число нитей перед первой областью задать равным 3,
// перед второй – равным 1. Внутри параллельных областей определить количество нитей и номер каждой нити,
// результат выдать на экран. Убедиться в правильности работы программы.

#include <iostream>
#include <omp.h>

// Если в задаче имелось в виду именно это (если условие в if false, то код выполняется не параллельно),
// то решение представлено ниже. А если же смысл задачи именно в НЕ выполнении кода совсем,
// то использовать конструкцию #pragma omp parallel if не получится. Пришлось бы внутри
// #pragma omp parallel использовать обычный if
int main() {
    if (omp_get_dynamic() != 0) {
        omp_set_dynamic(0);
    }

    omp_set_num_threads(3);
    #pragma omp parallel if(omp_get_max_threads() > 1)
    {
        std::string s = "Hello, world from thread " + std::to_string(omp_get_thread_num())
                        + " of " + std::to_string(omp_get_num_threads()) + ". \n";
        std::cout << s;
    }

    omp_set_num_threads(1);
    #pragma omp parallel if(omp_get_max_threads() > 1)
    {
        std::string s = "Hello, world from thread " + std::to_string(omp_get_thread_num())
                        + " of " + std::to_string(omp_get_num_threads()) + ". \n";
        std::cout << s;
    }
    return 0;
}
