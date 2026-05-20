#include "../BigDecimal.h"
#include <iostream>
#include <string>
using namespace std;

static int passed = 0, failed = 0;

static void check(const string& got, const string& expected) {
    if (got == expected) {
        passed++;
        cout << "OK    " << got << "\n";
    } else {
        failed++;
        cout << "FAIL  got: " << got << "  expected: " << expected << "\n";
    }
}

static void checkBool(bool got, bool expected) {
    check(got ? "valid" : "invalid", expected ? "valid" : "invalid");
}

static string parse(const string& s) {
    BigDecimal x;
    return x.fromString(s) ? x.toString() : "INVALID";
}

void runTests() {
    auto bd = [](const string& s) { BigDecimal x; x.fromString(s); return x; };

    cout << "-- parse valid --\n";
    check(parse("42"),          "42");
    check(parse("-42"),         "-42");
    check(parse("-0"),          "0");
    check(parse("007"),         "7");
    check(parse("1.5"),         "1.5");
    check(parse("-0.001"),      "-0.001");
    check(parse("100.00"),      "100");
    check(parse("1.50"),        "1.5");
    check(parse("000123.4500"), "123.45");
    check(parse("-.5"),         "-0.5");
    check(parse("1e3"),         "1000");
    check(parse("1e+3"),        "1000");
    check(parse("1e-3"),        "0.001");
    check(parse("-1e3"),        "-1000");
    check(parse("1.2e3"),       "1200");
    check(parse("1.2e-3"),      "0.0012");
    check(parse("0e100"),       "0");
    check(parse("0.0"),         "0");

    cout << "-- parse invalid --\n";
    checkBool(parse("") == "INVALID",     true);
    checkBool(parse("-") == "INVALID",    true);
    checkBool(parse("+5") == "INVALID",   true);
    checkBool(parse(".5") == "INVALID",   true);
    checkBool(parse("5.") == "INVALID",   true);
    checkBool(parse("1..2") == "INVALID", true);
    checkBool(parse("e10") == "INVALID",  true);
    checkBool(parse("1e") == "INVALID",   true);
    checkBool(parse("1e-") == "INVALID",  true);
    checkBool(parse("1e1.2") == "INVALID",true);
    checkBool(parse("1ee2") == "INVALID", true);
    checkBool(parse("abc") == "INVALID",  true);
    checkBool(parse("1 2") == "INVALID",  true);
    checkBool(parse("1,5") == "INVALID",  true);
    checkBool(parse("--1") == "INVALID",  true);

    cout << "-- add --\n";
    check(addBigDecimal(bd("1"),    bd("2")).toString(),    "3");
    check(addBigDecimal(bd("-2"),   bd("-3")).toString(),   "-5");
    check(addBigDecimal(bd("-5"),   bd("5")).toString(),    "0");
    check(addBigDecimal(bd("12.3"), bd("0.45")).toString(), "12.75");
    check(addBigDecimal(bd("1.2e3"),bd("3.4e2")).toString(),"1540");
    check(addBigDecimal(bd("99999"),bd("1")).toString(),    "100000");

    cout << "-- sub --\n";
    check(subBigDecimal(bd("10"),   bd("5")).toString(),   "5");
    check(subBigDecimal(bd("5"),    bd("10")).toString(),  "-5");
    check(subBigDecimal(bd("12.3"), bd("0.45")).toString(),"11.85");
    check(subBigDecimal(bd("-10"),  bd("5")).toString(),   "-15");
    check(subBigDecimal(bd("10"),   bd("-5")).toString(),  "15");

    cout << "-- mul --\n";
    check(mulBigDecimal(bd("2"),   bd("3")).toString(),   "6");
    check(mulBigDecimal(bd("-2"),  bd("3")).toString(),   "-6");
    check(mulBigDecimal(bd("-2"),  bd("-3")).toString(),  "6");
    check(mulBigDecimal(bd("1.5"), bd("2")).toString(),   "3");
    check(mulBigDecimal(bd("0.1"), bd("0.1")).toString(), "0.01");

    cout << "-- div --\n";
    check(divBigDecimal(bd("6"),  bd("2"), 10).toStringTruncated(10), "3");
    check(divBigDecimal(bd("-6"), bd("2"), 10).toStringTruncated(10), "-3");
    check(divBigDecimal(bd("1"),  bd("4"), 10).toStringTruncated(10), "0.25");
    check(divBigDecimal(bd("0"),  bd("5"), 10).toStringTruncated(10), "0");

    cout << "-- sqrt --\n";
    check(sqrtBigDecimal(bd("4"),   10).toStringTruncated(10), "2");
    check(sqrtBigDecimal(bd("9"),   10).toStringTruncated(10), "3");
    check(sqrtBigDecimal(bd("0.25"),10).toStringTruncated(10), "0.5");
    string s2 = sqrtBigDecimal(bd("2"), 10).toStringTruncated(10);
    checkBool(s2.substr(0,6) == "1.4142", true);
}

int main() {
    runTests();
    cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed > 0 ? 1 : 0;
}
