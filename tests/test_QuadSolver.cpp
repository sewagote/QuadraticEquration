#include "../QuadSolver.h"
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

    cout << passed << " тестов выполнено\n";
    return 0;
}
