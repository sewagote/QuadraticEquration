#pragma once
#include "BigDecimal.h"
#include <string>

struct QuadResult {
    enum class Status       { OK, INF, COMPLEX, WRONG };
    enum class EquationType { QUADRATIC, LINEAR };

    Status       status;
    EquationType equationType;
    int          rootCount;   // 0, 1 или 2

    // Вещественные корни (при status == OK)
    std::string root1;        // меньший корень (rootCount >= 1)
    std::string root2;        // больший корень (rootCount == 2)

    // Комплексные корни (при status == COMPLEX): x = re ± im*i
    std::string complexRe;    // вещественная часть -b/(2a)
    std::string complexIm;    // мнимая часть sqrt(-D)/(2a), всегда >= 0
};

class QuadSolver {
public:
    // Решает уравнение ax^2 + bx + c = 0.
    // Принимает коэффициенты строками, возвращает заполненный QuadResult.
    QuadResult solve(const std::string& sa,
                     const std::string& sb,
                     const std::string& sc) const;

private:
    static constexpr int PRECISION = 60;

    // Решает линейное уравнение bx + c = 0 (вызывается когда a == 0).
    // Если b == 0 и c == 0 — INF, если b == 0 и c != 0 — NO_SOLUTION (линейное),
    // иначе один корень x = -c/b.
    QuadResult solveLinear(const BigDecimal& b,
                           const BigDecimal& c) const;
};
