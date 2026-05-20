#pragma once
#include <string>

// Число с произвольной точностью.
// Хранится как: (isNegative ? -1 : 1) * digits * 10^exponent10
// digits — строка значащих цифр без ведущих и хвостовых нулей.
// exponent10 — показатель степени десятки, знаковая строка.
struct BigDecimal {
    std::string digits;
    std::string exponent10;
    bool isNegative;

    // Инициализирует нулём.
    BigDecimal();

    // Возвращает true если число равно нулю.
    bool isZero() const;

    // Приводит к каноническому виду: убирает хвостовые нули из digits,
    // увеличивая exponent10 на каждый удалённый ноль.
    void normalize();

    // Парсит строку формата [-]digits[.digits][e[+-]digits].
    // Возвращает false если формат невалиден.
    bool fromString(const std::string& s);

    // Возвращает точное десятичное представление числа.
    std::string toString() const;

    // Возвращает десятичное представление с усечением дробной части
    // до precision знаков. Хвостовые нули отбрасываются.
    std::string toStringTruncated(int precision) const;
};

// Убирает ведущие нули и лишний знак из числа-строки. "-00" -> "0", "+5" -> "5".
std::string normalizeString(std::string x);

// Сравнивает два знаковых числа-строки. Возвращает 1, -1 или 0.
int compareSignedStrings(std::string x, std::string y);

// Складывает два знаковых числа-строки с учётом знаков.
std::string addSignedStrings(std::string x, std::string y);

// Вычитает y из x для знаковых чисел-строк. Реализован через addSignedStrings со сменой знака y.
std::string subSignedStrings(std::string x, std::string y);

// Складывает два BigDecimal. Перед сложением выравнивает экспоненты,
// дописывая нули к digits числа с большей экспонентой.
BigDecimal addBigDecimal(BigDecimal a, BigDecimal b);

// Вычитает b из a. Реализован через addBigDecimal со сменой знака b.
BigDecimal subBigDecimal(BigDecimal a, BigDecimal b);

// Перемножает два BigDecimal. digits перемножаются через mul(),
// экспоненты складываются.
BigDecimal mulBigDecimal(const BigDecimal& a, const BigDecimal& b);

// Делит a на b с точностью precision значащих цифр.
// Дописывает (precision + |b.digits|) нулей к делимому, затем делит целочисленно.
BigDecimal divBigDecimal(BigDecimal a, BigDecimal b, int precision = 100);

// Вычисляет квадратный корень методом Ньютона с точностью precision знаков.
// Начальное приближение берётся через sqrt(double).
// Итерирует x = (x + a/x) / 2 до |x_new - x| < 10^(-(precision+5)).
BigDecimal sqrtBigDecimal(const BigDecimal& a, int precision = 60);
