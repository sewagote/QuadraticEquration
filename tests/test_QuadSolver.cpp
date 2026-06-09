#include "QuadSolver.h"
#include <iostream>
#include <string>
using namespace std;

static int passed = 0, failed = 0;

static void test(const string& a, const string& b, const string& c) {
    QuadResult r = QuadSolver().solve(a, b, c);

    cout << a << "x² + " << b << "x + " << c << "\n";

    switch (r.status) {
        case QuadResult::Status::OK:
            cout << "  Тип:      " << (r.equationType == QuadResult::EquationType::QUADRATIC ? "квадратное" : "линейное") << "\n";
            cout << "  x1 = " << r.root1 << "\n";
            if (r.rootCount == 2)
                cout << "  x2 = " << r.root2 << "\n";
            cout << "  Результат: OK\n";
            break;
        case QuadResult::Status::COMPLEX:
            cout << "  Тип:      квадратное\n";
            cout << "  x1 = " << r.complexRe << " + " << r.complexIm << "i\n";
            cout << "  x2 = " << r.complexRe << " - " << r.complexIm << "i\n";
            cout << "  Результат: OK (комплексные)\n";
            break;
        case QuadResult::Status::INF:
            cout << "  Результат: INF\n";
            break;
        case QuadResult::Status::WRONG:
            cout << "  Результат: WRONG\n";
            break;
    }
    cout << "\n";
    passed++;
}

// --- проверки с ожидаемым результатом (используются для больших чисел) ---
static void reportCase(bool ok, const string& title,
                       const string& got, const string& expected) {
    if (ok) {
        passed++;
        cout << "OK    " << title << "\n";
    } else {
        failed++;
        cout << "FAIL  " << title << "  got: " << got
             << "  expected: " << expected << "\n";
    }
}

static void expectRoots(const string& a, const string& b, const string& c,
                        const string& e1, const string& e2) {
    QuadResult r = QuadSolver().solve(a, b, c);
    string got = "count=" + to_string(r.rootCount) + " " + r.root1
               + (r.rootCount == 2 ? " " + r.root2 : "");
    bool ok = r.status == QuadResult::Status::OK && r.rootCount == 2
              && r.root1 == e1 && r.root2 == e2;
    reportCase(ok, "два корня: " + e1 + ", " + e2, got, e1 + " " + e2);
}

static void expectRoot(const string& a, const string& b, const string& c,
                       const string& e) {
    QuadResult r = QuadSolver().solve(a, b, c);
    string got = "count=" + to_string(r.rootCount) + " " + r.root1;
    bool ok = r.status == QuadResult::Status::OK && r.rootCount == 1
              && r.root1 == e;
    reportCase(ok, "двойной корень: " + e, got, e);
}

static void expectComplex(const string& a, const string& b, const string& c,
                          const string& re, const string& im) {
    QuadResult r = QuadSolver().solve(a, b, c);
    string got = r.complexRe + " +- " + r.complexIm + "i";
    bool ok = r.status == QuadResult::Status::COMPLEX
              && r.complexRe == re && r.complexIm == im;
    reportCase(ok, "комплексные: " + re + " +- " + im + "i", got,
               re + " +- " + im + "i");
}

int main() {
    cout << "-- два вещественных корня --\n\n";
    test("1", "-3", "2");
    test("1", "5",  "6");
    test("1", "-1", "-2");
    test("2", "-3", "1");
    test("1", "0",  "-4");
    test("-1","5",  "-6");
    test("1e0","-3e0","2e0");
    test("0.5","-1.5","1");

    cout << "-- один корень (D=0) --\n\n";
    test("1", "2",  "1");
    test("1", "0",  "0");
    test("4", "-4", "1");

    cout << "-- комплексные корни --\n\n";
    test("1", "0", "1");
    test("1", "1", "1");
    test("1", "2", "5");
    test("4", "0", "1");

    cout << "-- линейное (a=0) --\n\n";
    test("0", "2",  "-6");
    test("0", "4",  "-1");
    test("0", "1",  "5");

    cout << "-- особые случаи --\n\n";
    test("0", "0", "0");
    test("0", "0", "5");
    test("abc", "1", "2");
    test("1000000000000000000000","-2000000000000000000000","1000000000000000000000");
    test("1", "0", "-2");

    cout << "-- большие числа (с проверкой корней) --\n\n";
    // два целых корня: 10^12 и 10^12 + 5
    expectRoots("1", "-2000000000005", "1000000000005000000000000",
                "1000000000000", "1000000000005");
    // двойной корень x = 1, коэффициенты порядка 10^21
    expectRoot("1000000000000000000000", "-2000000000000000000000",
               "1000000000000000000000", "1");
    // двойной корень x = 10^15: (x - 10^15)^2 = 0
    expectRoot("1", "-2000000000000000", "1000000000000000000000000000000",
               "1000000000000000");
    // комплексные корни +-10^15 i: x^2 + 10^30 = 0
    expectComplex("1", "0", "1000000000000000000000000000000",
                  "0", "1000000000000000");
    // научная нотация с заглавной E: x^2 - 3x + 2 = 0  -> корни 1 и 2
    expectRoots("1E30", "-3E30", "2E30", "1", "2");
    // ~150-значные коэффициенты: корни 10^12 и 10^12 + 5
    expectRoots(
        "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
        "-20000000000050000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
        "10000000000050000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
        "1000000000000", "1000000000005");

    cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed > 0 ? 1 : 0;
}