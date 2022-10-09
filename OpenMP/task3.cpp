// Написать программу, в которой объявить и присвоить начальные значения целочисленным переменным: a и b,
// определить две параллельные области, первая – с числом нитей 2, вторая – с числом нитей 4.
// Для первой области переменные a и b объявить защищенными, с режимом доступа private и firstprivate, соответственно,
// внутри области определить номер нити и выполнить увеличение значения переменных на это величину.
// Выдать на экран значения переменных до входа в параллельную область, внутри параллельной области
// и после выхода из параллельной области. Для второй области переменную a объявить общей,
// переменную b объявить защищенной, с режимом доступа private, определить номер нити и выполнить
// уменьшение значения переменных на это величину. Выдать на экран значения переменных до входа в параллельную область,
// внутри параллельной области и после выхода из параллельной области.

#include <iostream>
#include <omp.h>

int main() {

    int a = 0;
    int b = 0;

    std::cout << "a = " + std::to_string(a) + "; b = " + std::to_string(b) + ". \n\n";

    #pragma omp parallel num_threads(2) firstprivate(b) private(a)
    {
        a = 0;
        a += omp_get_thread_num();
        b += omp_get_thread_num();

        std::cout << "a = " + std::to_string(a) + "; b = " + std::to_string(b) + ". \n";
    }

    std::cout <<  "\na = " + std::to_string(a) + "; b = " + std::to_string(b) + ". \n\n";

    #pragma omp parallel num_threads(4) shared(a) private(b)
    {
        b = 0;
        #pragma omp atomic
        a -= omp_get_thread_num();
        b -= omp_get_thread_num();

        std::cout << "a = " + std::to_string(a) + "; b = " + std::to_string(b) + ". \n";
    }

    std::cout << "\na = " + std::to_string(a) + "; b = " + std::to_string(b) + ". \n";
    return 0;
}
