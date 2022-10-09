// Написать параллельную программу возведения числа 210 в квадрат без операции умножения.
// Пояснение: Квадрат натурального числа N равен сумме первых N нечетных чисел.
// Например, 3^2 = 9 это 1 + 3 + 5 = 9; 5^2 = 25 это 1 + 3 + 5 + 7 + 9 = 25;

#include <iostream>

int main() {
    int N = 210;
    int result = 0;

    #pragma omp parallel for schedule(guided, 4) reduction(+: result)
    for (int i = 0; i < N; i++) {
        result += i + i + 1;
    }

    std::cout << std::to_string(N) + "^2 = " + std::to_string(result) + "\n";

    return 0;
}
