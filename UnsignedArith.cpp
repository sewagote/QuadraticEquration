#include "UnsignedArith.h"
#include <algorithm>
#include <vector>
using namespace std;

int compareStrings(string x, string y) {
    size_t posX = 0, posY = 0;
    while (posX < x.size() && x[posX] == '0') posX++;
    while (posY < y.size() && y[posY] == '0') posY++;
    x = (posX == x.size()) ? "0" : x.substr(posX);
    y = (posY == y.size()) ? "0" : y.substr(posY);
    if (x.size() > y.size()) return 1;
    if (x.size() < y.size()) return -1;
    for (size_t i = 0; i < x.size(); i++) {
        if (x[i] > y[i]) return 1;
        if (x[i] < y[i]) return -1;
    }
    return 0;
}

string add(string x, string y) {
    if (y.size() > x.size()) swap(x, y);
    string result;
    int carry = 0;
    size_t n1 = x.size(), n2 = y.size();
    for (size_t i = 0; i < n1; i++) {
        int digitX = x[n1 - 1 - i] - '0';
        int digitY = (i < n2) ? y[n2 - 1 - i] - '0' : 0;
        int sum = digitX + digitY + carry;
        carry = sum / 10;
        result.push_back((sum % 10) + '0');
    }
    if (carry > 0) result.push_back(carry + '0');
    reverse(result.begin(), result.end());
    size_t pos = result.find_first_not_of('0');
    return (pos == string::npos) ? "0" : result.substr(pos);
}

string sub(string x, string y) {
    if (compareStrings(x, y) == -1) swap(x, y);
    string result;
    int borrow = 0;
    size_t n1 = x.size(), n2 = y.size();
    for (size_t i = 0; i < n1; i++) {
        int digitX = x[n1 - 1 - i] - '0';
        int digitY = (i < n2) ? y[n2 - 1 - i] - '0' : 0;
        int s;
        if (digitX - borrow < digitY) { s = digitX - borrow + 10 - digitY; borrow = 1; }
        else                           { s = digitX - borrow - digitY;       borrow = 0; }
        result.push_back(s + '0');
    }
    reverse(result.begin(), result.end());
    size_t pos = result.find_first_not_of('0');
    return (pos == string::npos) ? "0" : result.substr(pos);
}

string mul(const string& x, const string& y) {
    if (x == "0" || y == "0") return "0";
    size_t n1 = x.size(), n2 = y.size();
    vector<int> res(n1 + n2, 0);
    for (int i = (int)n1 - 1; i >= 0; i--) {
        for (int j = (int)n2 - 1; j >= 0; j--) {
            int p = (x[i] - '0') * (y[j] - '0');
            int sum = p + res[i + j + 1];
            res[i + j + 1] = sum % 10;
            res[i + j]    += sum / 10;
        }
    }
    string result;
    for (int d : res)
        if (!(result.empty() && d == 0)) result.push_back(d + '0');
    return result.empty() ? "0" : result;
}

pair<string, string> divmod(const string& x, const string& y) {
    if (compareStrings(x, y) == -1) return {"0", x};
    string quotient;
    string current = "0";
    for (size_t i = 0; i < x.size(); i++) {
        if (current == "0") current = string(1, x[i]);
        else                current.push_back(x[i]);

        int lo = 0, hi = 9, digit = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (compareStrings(mul(y, to_string(mid)), current) <= 0) { digit = mid; lo = mid + 1; }
            else hi = mid - 1;
        }
        quotient.push_back(digit + '0');
        current = sub(current, mul(y, to_string(digit)));
    }
    size_t pos = quotient.find_first_not_of('0');
    quotient = (pos == string::npos) ? "0" : quotient.substr(pos);
    return {quotient, current};
}
