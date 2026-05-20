#include "QuadSolver.h"
#include "BigDecimal.h"
using namespace std;

// Сбрасывает флаг isNegative у нуля, чтобы не получалось "-0".
static void fixNegativeZero(BigDecimal& x) {
    if (x.isZero()) x.isNegative = false;
}

QuadResult QuadSolver::solveLinear(const BigDecimal& b,
                                   const BigDecimal& c) const {
    QuadResult res;
    res.equationType = QuadResult::EquationType::LINEAR;
    if (b.isZero()) {
        res.status    = c.isZero() ? QuadResult::Status::INF
                                   : QuadResult::Status::WRONG;
        res.rootCount = 0;
        return res;
    }
    BigDecimal x = divBigDecimal(c, b, PRECISION);
    x.isNegative  = !x.isNegative;
    fixNegativeZero(x);

    res.status    = QuadResult::Status::OK;
    res.rootCount = 1;
    res.root1     = x.toStringTruncated(PRECISION);
    return res;
}

QuadResult QuadSolver::solve(const string& sa,
                             const string& sb,
                             const string& sc) const {
    QuadResult res;
    BigDecimal a, b, c;

    if (!a.fromString(sa) || !b.fromString(sb) || !c.fromString(sc)) {
        res.status    = QuadResult::Status::WRONG;
        res.rootCount = 0;
        return res;
    }

    if (a.isZero()) return solveLinear(b, c);

    res.equationType = QuadResult::EquationType::QUADRATIC;

    // D = b^2 - 4ac
    BigDecimal four;  four.fromString("4");
    BigDecimal D = subBigDecimal(mulBigDecimal(b, b),
                                 mulBigDecimal(four, mulBigDecimal(a, c)));

    int dSign = D.isZero() ? 0 : (D.isNegative ? -1 : 1);

    BigDecimal two;  two.fromString("2");
    BigDecimal twoA = mulBigDecimal(two, a);

    if (dSign < 0) {
        // Комплексные корни: x = -b/(2a) ± i * sqrt(-D)/(2a)
        BigDecimal negD = D;
        negD.isNegative = false;  // -D (модуль)

        BigDecimal re = divBigDecimal(b, twoA, PRECISION);
        re.isNegative = !re.isNegative;
        fixNegativeZero(re);

        BigDecimal im = divBigDecimal(sqrtBigDecimal(negD, PRECISION), twoA, PRECISION);
        im.isNegative = false;    // мнимая часть всегда положительна
        fixNegativeZero(im);

        res.status    = QuadResult::Status::COMPLEX;
        res.rootCount = 2;
        res.complexRe = re.toStringTruncated(PRECISION);
        res.complexIm = im.toStringTruncated(PRECISION);
        return res;
    }

    if (dSign == 0) {
        // x = -b / (2a)
        BigDecimal x = divBigDecimal(b, twoA, PRECISION);
        x.isNegative  = !x.isNegative;
        fixNegativeZero(x);

        res.status    = QuadResult::Status::OK;
        res.rootCount = 1;
        res.root1     = x.toStringTruncated(PRECISION);
        return res;
    }

    // x = (-b ± sqrt(D)) / (2a)
    BigDecimal sqrtD = sqrtBigDecimal(D, PRECISION);

    BigDecimal negB = b;
    if (!negB.isZero()) negB.isNegative = !negB.isNegative;

    BigDecimal x1 = divBigDecimal(addBigDecimal(negB, sqrtD), twoA, PRECISION);
    BigDecimal x2 = divBigDecimal(subBigDecimal(negB, sqrtD), twoA, PRECISION);
    fixNegativeZero(x1);
    fixNegativeZero(x2);

    // Сортируем: если x1 > x2 — меняем местами
    {
        BigDecimal diff = subBigDecimal(x1, x2);
        if (!diff.isZero() && !diff.isNegative) swap(x1, x2);
    }

    res.status    = QuadResult::Status::OK;
    res.rootCount = 2;
    res.root1     = x1.toStringTruncated(PRECISION);
    res.root2     = x2.toStringTruncated(PRECISION);
    return res;
}
