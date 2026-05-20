#pragma once
#include <string>
#include <utility>

// Сравнивает два беззнаковых числа-строки.
// Возвращает 1 если x > y, -1 если x < y, 0 если равны.
// Ведущие нули игнорируются.
int compareStrings(std::string x, std::string y);

// Складывает два беззнаковых числа-строки поразрядно справа налево с переносом.
std::string add(std::string x, std::string y);

// Вычитает два беззнаковых числа-строки, всегда возвращает |x - y|.
// Если x < y — операнды меняются местами перед вычитанием.
std::string sub(std::string x, std::string y);

// Перемножает два беззнаковых числа-строки в столбик.
// Каждая пара цифр x[i] * y[j] накапливается в векторе позиций i+j, i+j+1.
std::string mul(const std::string& x, const std::string& y);

// Делит x на y в столбик, возвращает {частное, остаток}.
// На каждом шаге цифра частного ищется бинарным поиском по [0, 9].
// Предусловие: y != "0".
std::pair<std::string, std::string> divmod(const std::string& x, const std::string& y);
