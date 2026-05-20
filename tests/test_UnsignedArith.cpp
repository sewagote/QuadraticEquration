#include "../UnsignedArith.h"
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

static void checkInt(int got, int expected) {
    check(to_string(got), to_string(expected));
}

void runTests() {
    cout << "-- compareStrings --\n";
    checkInt(compareStrings("123", "123"),  0);
    checkInt(compareStrings("10",  "9"),    1);
    checkInt(compareStrings("9",   "10"),  -1);
    checkInt(compareStrings("007", "7"),    0);
    checkInt(compareStrings("000", "0"),    0);

    cout << "-- add --\n";
    check(add("0",   "0"),   "0");
    check(add("1",   "2"),   "3");
    check(add("99",  "1"),   "100");
    check(add("999", "999"), "1998");
    check(add("1000","1"),   "1001");
    check(add("99999999999999999999", "1"), "100000000000000000000");

    cout << "-- sub --\n";
    check(sub("5",   "3"),  "2");
    check(sub("100", "1"),  "99");
    check(sub("42",  "42"), "0");
    check(sub("3",   "5"),  "2");
    check(sub("1000","999"),"1");

    cout << "-- mul --\n";
    check(mul("0",   "99999"), "0");
    check(mul("1",   "12345"), "12345");
    check(mul("9",   "9"),     "81");
    check(mul("123", "456"),   "56088");
    check(mul("999", "999"),   "998001");
    check(mul("10000000000","10000000000"), "100000000000000000000");

    cout << "-- divmod --\n";
    auto [q1,r1] = divmod("3",  "5");   check(q1,"0");  check(r1,"3");
    auto [q2,r2] = divmod("100","10");  check(q2,"10"); check(r2,"0");
    auto [q3,r3] = divmod("7",  "3");   check(q3,"2");  check(r3,"1");
    auto [q4,r4] = divmod("100000000000","3");
    check(q4,"33333333333"); check(r4,"1");
    string x="987654321", y="12345";
    auto [q5,r5] = divmod(x, y);
    check(add(mul(q5,y),r5), x);
}

int main() {
    runTests();
    cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed > 0 ? 1 : 0;
}
