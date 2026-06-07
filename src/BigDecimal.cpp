#include "BigDecimal.h"
#include "UnsignedArith.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
using namespace std;

string normalizeString(string x) {
    if (x.empty()) return "0";
    if (x[0] == '+') x = x.substr(1);
    if (x[0] == '-') {
        string n = x.substr(1);
        size_t p = n.find_first_not_of('0');
        return (p == string::npos) ? "0" : "-" + n.substr(p);
    }
    size_t p = x.find_first_not_of('0');
    return (p == string::npos) ? "0" : x.substr(p);
}

int compareSignedStrings(string x, string y) {
    x = normalizeString(x);
    y = normalizeString(y);
    bool xNeg = (!x.empty() && x[0] == '-');
    bool yNeg = (!y.empty() && y[0] == '-');
    string absX = xNeg ? x.substr(1) : x;
    string absY = yNeg ? y.substr(1) : y;
    if (xNeg && !yNeg) return -1;
    if (!xNeg && yNeg) return 1;
    if (!xNeg && !yNeg) return compareStrings(absX, absY);
    int cmp = compareStrings(absX, absY);
    if (cmp == 1) return -1;
    if (cmp == -1) return 1;
    return 0;
}

string addSignedStrings(string x, string y) {
    x = normalizeString(x);
    y = normalizeString(y);
    bool xNeg = (!x.empty() && x[0] == '-');
    bool yNeg = (!y.empty() && y[0] == '-');
    string absX = xNeg ? x.substr(1) : x;
    string absY = yNeg ? y.substr(1) : y;
    if (xNeg == yNeg) {
        string res = normalizeString(add(absX, absY));
        if (res == "0") return "0";
        return xNeg ? "-" + res : res;
    }
    int cmp = compareStrings(absX, absY);
    if (cmp == 0) return "0";
    if (cmp > 0) {
        string res = normalizeString(sub(absX, absY));
        return (res == "0") ? "0" : (xNeg ? "-" + res : res);
    }
    string res = normalizeString(sub(absY, absX));
    return (res == "0") ? "0" : (yNeg ? "-" + res : res);
}

string subSignedStrings(string x, string y) {
    y = normalizeString(y);
    if (y == "0") return normalizeString(x);
    if (!y.empty() && y[0] == '-') y = y.substr(1);
    else y = '-' + y;
    return addSignedStrings(x, y);
}

// Проверяет мантиссу: допустимы цифры, один знак '-' первым символом,
// одна точка не крайней позиции.
static bool isValidMantissa(const string& s) {
    if (s.empty()) return false;
    int dotCount = 0, digitCount = 0;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '.') {
            dotCount++;
            if (i == 0 || i == s.size() - 1 || dotCount > 1) return false;
        } else if (s[i] == '-') {
            if (i != 0) return false;
        } else if (s[i] >= '0' && s[i] <= '9') {
            digitCount++;
        } else return false;
    }
    return digitCount > 0;
}

// Проверяет показатель экспоненты: цифры, опциональный знак первым символом, без точки.
static bool isExponentValid(const string& s) {
    if (s.empty()) return false;
    int digitCount = 0;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '.') return false;
        if (s[i] == '+' || s[i] == '-') { if (i != 0) return false; }
        else if (s[i] >= '0' && s[i] <= '9') digitCount++;
        else return false;
    }
    return digitCount > 0;
}

// Проверяет всю строку: разбивает по 'e' на мантиссу и экспоненту, валидирует обе части.
static bool validateDecimalString(const string& s) {
    if (s.empty()) return false;
    size_t ePos = s.find_first_of('e');
    if (ePos != string::npos) {
        if (s.find_first_of('e', ePos + 1) != string::npos) return false;
        return isValidMantissa(s.substr(0, ePos)) && isExponentValid(s.substr(ePos + 1));
    }
    return isValidMantissa(s);
}

// Дописывает delta нулей к строке digits. Используется при выравнивании экспонент.
static bool appendZeros(string& digits, const string& delta) {
    string d = normalizeString(delta);
    if (d.empty() || d[0] == '-') return false;
    if (d == "0") return true;
    size_t count = stoull(d);
    digits.append(count, '0');
    return true;
}

BigDecimal::BigDecimal() : digits("0"), exponent10("0"), isNegative(false) {}

bool BigDecimal::isZero() const { return digits == "0"; }

void BigDecimal::normalize() {
    digits = normalizeString(digits);
    if (digits == "0") { exponent10 = "0"; isNegative = false; return; }
    while (!digits.empty() && digits.back() == '0') {
        digits.pop_back();
        exponent10 = addSignedStrings(exponent10, "1");
    }
    exponent10 = normalizeString(exponent10);
}

bool BigDecimal::fromString(const string& sInput) {
    // Принимаем как строчную 'e', так и заглавную 'E' в научной нотации.
    string s = sInput;
    for (char& ch : s) if (ch == 'E') ch = 'e';

    if (!validateDecimalString(s)) return false;
    size_t ePos = s.find_first_of('e');
    string mantissa         = (ePos != string::npos) ? s.substr(0, ePos) : s;
    string explicitExponent = (ePos != string::npos) ? s.substr(ePos + 1) : "0";

    if (mantissa[0] == '-') { isNegative = true; mantissa = mantissa.substr(1); }
    else isNegative = false;

    size_t dotPos = mantissa.find_first_of('.');
    string mantissaExponent;
    if (dotPos != string::npos) {
        string right = mantissa.substr(dotPos + 1);
        digits = mantissa.substr(0, dotPos) + right;
        mantissaExponent = "-" + to_string(right.size());
    } else {
        digits = mantissa;
        mantissaExponent = "0";
    }

    explicitExponent = normalizeString(explicitExponent);
    exponent10 = addSignedStrings(mantissaExponent, explicitExponent);
    exponent10 = normalizeString(exponent10);
    digits = normalizeString(digits);
    normalize();
    return true;
}

string BigDecimal::toString() const {
    if (digits == "0") return "0";
    string exp  = normalizeString(exponent10);
    string sign = isNegative ? "-" : "";
    if (exp == "0") return sign + digits;
    if (exp[0] == '-') {
        string absExp  = exp.substr(1);
        string digSize = to_string(digits.size());
        int cmp = compareStrings(absExp, digSize);
        if (cmp == -1) {
            size_t shift = stoull(absExp);
            string dig = digits;
            dig.insert(dig.size() - shift, ".");
            return sign + dig;
        } else if (cmp == 0) {
            return sign + "0." + digits;
        } else {
            string zerosCount = sub(absExp, digSize);
            string dig = "0.";
            appendZeros(dig, zerosCount);
            dig += digits;
            return sign + dig;
        }
    } else {
        string dig = digits;
        appendZeros(dig, exp);
        return sign + dig;
    }
}

string BigDecimal::toStringTruncated(int precision) const {
    if (isZero()) return "0";
    string exp  = normalizeString(exponent10);
    string sign = isNegative ? "-" : "";

    if (exp == "0" || exp[0] != '-') {
        string dig = digits;
        if (exp != "0") appendZeros(dig, exp);
        return sign + dig;
    }

    string absExp  = exp.substr(1);
    string digSize = to_string(digits.size());
    string intPart, fracPart;
    int cmp = compareStrings(absExp, digSize);
    if (cmp == -1) {
        size_t shift = stoull(absExp);
        intPart  = digits.substr(0, digits.size() - shift);
        fracPart = digits.substr(digits.size() - shift);
    } else if (cmp == 0) {
        intPart  = "0";
        fracPart = digits;
    } else {
        intPart = "0";
        string zeros(stoull(sub(absExp, digSize)), '0');
        fracPart = zeros + digits;
    }

    if ((int)fracPart.size() > precision) fracPart = fracPart.substr(0, precision);
    while (!fracPart.empty() && fracPart.back() == '0') fracPart.pop_back();
    if (fracPart.empty()) return sign + intPart;
    return sign + intPart + "." + fracPart;
}

// Выравнивает экспоненты a и b, дописывая нули к digits числа с большей экспонентой,
// чтобы оба имели одинаковый exponent10 перед сложением/вычитанием.
static void alignExponents(BigDecimal& a, BigDecimal& b) {
    int cmp = compareSignedStrings(a.exponent10, b.exponent10);
    if (cmp == 0) return;
    if (cmp == 1) {
        string delta = subSignedStrings(a.exponent10, b.exponent10);
        if (appendZeros(a.digits, delta)) a.exponent10 = b.exponent10;
    } else {
        string delta = subSignedStrings(b.exponent10, a.exponent10);
        if (appendZeros(b.digits, delta)) b.exponent10 = a.exponent10;
    }
}

BigDecimal addBigDecimal(BigDecimal a, BigDecimal b) {
    alignExponents(a, b);
    BigDecimal result;
    result.exponent10 = a.exponent10;
    if (a.isNegative == b.isNegative) {
        result.isNegative = a.isNegative;
        result.digits = add(a.digits, b.digits);
    } else {
        int cmp = compareStrings(a.digits, b.digits);
        if (cmp == 1)       { result.digits = sub(a.digits, b.digits); result.isNegative = a.isNegative; }
        else if (cmp == -1) { result.digits = sub(b.digits, a.digits); result.isNegative = b.isNegative; }
        else                { result.digits = "0"; result.isNegative = false; }
    }
    result.normalize();
    return result;
}

BigDecimal subBigDecimal(BigDecimal a, BigDecimal b) {
    if (!b.isZero()) b.isNegative = !b.isNegative;
    return addBigDecimal(a, b);
}

BigDecimal mulBigDecimal(const BigDecimal& a, const BigDecimal& b) {
    BigDecimal result;
    if (a.isZero() || b.isZero()) return result;
    result.digits     = mul(a.digits, b.digits);
    result.exponent10 = addSignedStrings(a.exponent10, b.exponent10);
    result.isNegative = (a.isNegative != b.isNegative);
    result.normalize();
    return result;
}

BigDecimal divBigDecimal(BigDecimal a, BigDecimal b, int precision) {
    BigDecimal result;
    if (b.isZero() || a.isZero()) return result;
    result.isNegative = (a.isNegative != b.isNegative);

    int extraZeros = precision + (int)b.digits.size();
    string dividend = a.digits;
    dividend.append(extraZeros, '0');

    auto [q, r] = divmod(dividend, b.digits);

    result.digits     = q;
    result.exponent10 = subSignedStrings(
        subSignedStrings(a.exponent10, b.exponent10),
        to_string(extraZeros)
    );
    result.normalize();
    return result;
}

BigDecimal sqrtBigDecimal(const BigDecimal& a, int precision) {
    BigDecimal zero;
    if (a.isZero() || a.isNegative) return zero;

    // Начальное приближение: берём мантиссу (первые цифры) и реальный порядок числа.
    // Десятичный порядок числа = exponent10 + (число цифр) - 1.
    // sqrt уменьшает порядок вдвое, поэтому начальное приближение
    // строим как (sqrt мантиссы) * 10^(порядок / 2).
    long long expVal = stoll(normalizeString(a.exponent10));
    long long order = expVal + (long long)a.digits.size() - 1;  // порядок a

    // мантисса в [1, 10): первая цифра . остальные
    string mant = a.digits.substr(0, min((size_t)15, a.digits.size()));
    double mantVal = stod(mant.substr(0, 1) + "." + (mant.size() > 1 ? mant.substr(1) : "0"));

    double rootMant = sqrt(mantVal);   // sqrt мантиссы, ~[1, 3.2)
    long long rootOrder = order / 2;   // половина порядка
    if (order % 2 != 0) {
        // нечётный порядок: домножаем мантиссу на sqrt(10)
        rootMant *= (order > 0) ? sqrt(10.0) : 1.0 / sqrt(10.0);
        if (order < 0) rootOrder = (order - 1) / 2;
    }

    char buf[64];
    snprintf(buf, sizeof(buf), "%.10e", rootMant);
    BigDecimal x;
    x.fromString(string(buf));
    // домножаем на 10^rootOrder через сдвиг экспоненты
    x.exponent10 = addSignedStrings(x.exponent10, to_string(rootOrder));
    x.normalize();
    if (x.isZero()) x.fromString("1");

    BigDecimal two;
    two.fromString("2");

    for (int i = 0; i < 200; i++) {
        BigDecimal ax   = divBigDecimal(a, x, precision + 20);
        BigDecimal xNew = divBigDecimal(addBigDecimal(x, ax), two, precision + 20);

        BigDecimal diff = subBigDecimal(xNew, x);
        diff.isNegative = false;

        BigDecimal eps;
        eps.digits     = "1";
        eps.exponent10 = to_string(-(precision + 5));

        x = xNew;
        if (diff.isZero() || compareSignedStrings(diff.exponent10, eps.exponent10) == -1)
            break;
    }
    return x;
}