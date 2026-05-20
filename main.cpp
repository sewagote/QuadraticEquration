#include <iostream>
#include "QuadSolver.h"
using namespace std;

// Форматирует комплексное число re ± im*i в строку.
static string formatComplex(const string& re, const string& im, bool plus) {
    string sign = plus ? " + " : " - ";
    return re + sign + im + "i";
}

static void printResult(const QuadResult& res) {
    switch (res.status) {
        case QuadResult::Status::OK: {
            string eqType = (res.equationType == QuadResult::EquationType::QUADRATIC)
                            ? "квадратное" : "линейное";
            cout << "Уравнение - " << eqType << "\n";
            cout << "Кол-во корней - " << res.rootCount << "\n";
            cout << "x1 = " << res.root1 << "\n";
            if (res.rootCount == 2)
                cout << "x2 = " << res.root2 << "\n";
            break;
        }
        case QuadResult::Status::COMPLEX:
            cout << "Уравнение - квадратное\n";
            cout << "Кол-во корней - 2 (комплексные)\n";
            cout << "x1 = " << formatComplex(res.complexRe, res.complexIm, true)  << "\n";
            cout << "x2 = " << formatComplex(res.complexRe, res.complexIm, false) << "\n";
            break;
        case QuadResult::Status::INF:
            cout << "INF\n";
            break;
        case QuadResult::Status::WRONG:
            cout << "WRONG\n";
            break;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    QuadSolver solver;

    while (true) {
        cout << "\n1 - Решить уравнение\n";
        cout << "2 - Выйти\n";
        cout << "> ";

        int choice;
        if (!(cin >> choice)) break;

        if (choice == 2) break;

        if (choice == 1) {
            cout << "Введите коэффициенты a, b, c через пробел: ";
            string sa, sb, sc;
            cin >> sa >> sb >> sc;
            cout << "\n";
            printResult(solver.solve(sa, sb, sc));
        }
    }

    return 0;
}
