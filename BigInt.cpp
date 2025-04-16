#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

class LargeInteger {
    string data;
public:
    LargeInteger(unsigned long long n = 0);
    LargeInteger(string &s);
    LargeInteger(const char *s);
    LargeInteger(LargeInteger &a);

    friend void halve(LargeInteger &a);
    friend bool isZero(const LargeInteger &);
    friend int size(const LargeInteger &);
    int at(const int) const;

    LargeInteger &operator=(const LargeInteger &);

    LargeInteger &operator++();
    LargeInteger operator++(int);
    LargeInteger &operator--();
    LargeInteger operator--(int);

    friend LargeInteger &operator+=(LargeInteger &, const LargeInteger &);
    friend LargeInteger operator+(const LargeInteger &, const LargeInteger &);
    friend LargeInteger operator-(const LargeInteger &, const LargeInteger &);
    friend LargeInteger &operator-=(LargeInteger &, const LargeInteger &);

    friend bool operator==(const LargeInteger &, const LargeInteger &);
    friend bool operator!=(const LargeInteger &, const LargeInteger &);
    friend bool operator>(const LargeInteger &, const LargeInteger &);
    friend bool operator>=(const LargeInteger &, const LargeInteger &);
    friend bool operator<(const LargeInteger &, const LargeInteger &);
    friend bool operator<=(const LargeInteger &, const LargeInteger &);

    friend LargeInteger &operator*=(LargeInteger &, const LargeInteger &);
    friend LargeInteger operator*(const LargeInteger &, const LargeInteger &);
    friend LargeInteger &operator/=(LargeInteger &, const LargeInteger &);
    friend LargeInteger operator/(const LargeInteger &, const LargeInteger &);

    friend LargeInteger operator%(const LargeInteger &, const LargeInteger &);
    friend LargeInteger &operator%=(LargeInteger &, const LargeInteger &);

    friend LargeInteger &operator^=(LargeInteger &, const LargeInteger &);
    friend LargeInteger operator^(LargeInteger &, const LargeInteger &);
    
    friend LargeInteger computeSquareRoot(LargeInteger &a);

    friend ostream &operator<<(ostream &, const LargeInteger &);
    friend istream &operator>>(istream &, LargeInteger &);

    friend LargeInteger calculateCatalan(int n);
    friend LargeInteger calculateFibonacci(int n);
    friend LargeInteger calculateFactorial(int n);
};

LargeInteger::LargeInteger(string &s) {
    data = "";
    int n = s.size();
    for (int i = n - 1; i >= 0; i--) {
        if (!isdigit(s[i]))
            throw("INVALID_INPUT");
        data.push_back(s[i] - '0');
    }
}

LargeInteger::LargeInteger(unsigned long long nr) {
    do {
        data.push_back(nr % 10);
        nr /= 10;
    } while (nr);
}

LargeInteger::LargeInteger(const char *s) {
    data = "";
    for (int i = strlen(s) - 1; i >= 0; i--) {
        if (!isdigit(s[i]))
            throw("INVALID_INPUT");
        data.push_back(s[i] - '0');
    }
}

LargeInteger::LargeInteger(LargeInteger &a) {
    data = a.data;
}

bool isZero(const LargeInteger &a) {
    return (a.data.size() == 1 && a.data[0] == 0);
}

int size(const LargeInteger &a) {
    return a.data.size();
}

int LargeInteger::at(const int index) const {
    if (data.size() <= index || index < 0)
        throw("INDEX_OUT_OF_RANGE");
    return data[index];
}

bool operator==(const LargeInteger &a, const LargeInteger &b) {
    return a.data == b.data;
}

bool operator!=(const LargeInteger &a, const LargeInteger &b) {
    return !(a == b);
}

bool operator<(const LargeInteger &a, const LargeInteger &b) {
    int n = size(a), m = size(b);
    if (n != m)
        return n < m;
    while (n--)
        if (a.data[n] != b.data[n])
            return a.data[n] < b.data[n];
    return false;
}

bool operator>(const LargeInteger &a, const LargeInteger &b) {
    return b < a;
}

bool operator>=(const LargeInteger &a, const LargeInteger &b) {
    return !(a < b);
}

bool operator<=(const LargeInteger &a, const LargeInteger &b) {
    return !(a > b);
}

LargeInteger &LargeInteger::operator=(const LargeInteger &a) {
    data = a.data;
    return *this;
}

LargeInteger &LargeInteger::operator++() {
    int i, n = data.size();
    for (i = 0; i < n && data[i] == 9; i++)
        data[i] = 0;
    if (i == n)
        data.push_back(1);
    else
        data[i]++;
    return *this;
}

LargeInteger LargeInteger::operator++(int) {
    LargeInteger temp;
    temp = *this;
    ++(*this);
    return temp;
}

LargeInteger &LargeInteger::operator--() {
    if (data[0] == 0 && data.size() == 1)
        throw("UNDERFLOW_ERROR");
    int i, n = data.size();
    for (i = 0; data[i] == 0 && i < n; i++)
        data[i] = 9;
    data[i]--;
    if (n > 1 && data[n - 1] == 0)
        data.pop_back();
    return *this;
}

LargeInteger LargeInteger::operator--(int) {
    LargeInteger temp;
    temp = *this;
    --(*this);
    return temp;
}

LargeInteger &operator+=(LargeInteger &a, const LargeInteger &b) {
    int carry = 0, sum, i;
    int n = size(a), m = size(b);
    if (m > n)
        a.data.append(m - n, 0);
    n = size(a);
    for (i = 0; i < n; i++) {
        if (i < m)
            sum = (a.data[i] + b.data[i]) + carry;
        else
            sum = a.data[i] + carry;
        carry = sum / 10;
        a.data[i] = (sum % 10);
    }
    if (carry)
        a.data.push_back(carry);
    return a;
}

LargeInteger operator+(const LargeInteger &a, const LargeInteger &b) {
    LargeInteger result;
    result = a;
    result += b;
    return result;
}

LargeInteger &operator-=(LargeInteger &a, const LargeInteger &b) {
    if (a < b)
        throw("UNDERFLOW_ERROR");
    int n = size(a), m = size(b);
    int i, borrow = 0, diff;
    for (i = 0; i < n; i++) {
        if (i < m)
            diff = a.data[i] - b.data[i] + borrow;
        else
            diff = a.data[i] + borrow;
        if (diff < 0)
            diff += 10, borrow = -1;
        else
            borrow = 0;
        a.data[i] = diff;
    }
    while (n > 1 && a.data[n - 1] == 0)
        a.data.pop_back(), n--;
    return a;
}

LargeInteger operator-(const LargeInteger &a, const LargeInteger &b) {
    LargeInteger result;
    result = a;
    result -= b;
    return result;
}

LargeInteger &operator*=(LargeInteger &a, const LargeInteger &b) {
    if (isZero(a) || isZero(b)) {
        a = LargeInteger();
        return a;
    }
    int n = a.data.size(), m = b.data.size();
    vector<int> v(n + m, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            v[i + j] += (a.data[i]) * (b.data[j]);
        }
    n += m;
    a.data.resize(v.size());
    for (int s, i = 0, t = 0; i < n; i++) {
        s = t + v[i];
        v[i] = s % 10;
        t = s / 10;
        a.data[i] = v[i];
    }
    for (int i = n - 1; i >= 1 && !v[i]; i--)
        a.data.pop_back();
    return a;
}

LargeInteger operator*(const LargeInteger &a, const LargeInteger &b) {
    LargeInteger result;
    result = a;
    result *= b;
    return result;
}

LargeInteger &operator/=(LargeInteger &a, const LargeInteger &b) {
    if (isZero(b))
        throw("DIVISION_BY_ZERO");
    if (a < b) {
        a = LargeInteger();
        return a;
    }
    if (a == b) {
        a = LargeInteger(1);
        return a;
    }
    int i, resultSize = 0, digit;
    int n = size(a), m = size(b);
    vector<int> result(n, 0);
    LargeInteger temp;
    for (i = n - 1; temp * 10 + a.data[i] < b; i--) {
        temp *= 10;
        temp += a.data[i];
    }
    for (; i >= 0; i--) {
        temp = temp * 10 + a.data[i];
        for (digit = 9; digit * b > temp; digit--);
        temp -= digit * b;
        result[resultSize++] = digit;
    }
    a.data.resize(result.size());
    for (i = 0; i < resultSize; i++)
        a.data[i] = result[resultSize - i - 1];
    a.data.resize(resultSize);
    return a;
}

LargeInteger operator/(const LargeInteger &a, const LargeInteger &b) {
    LargeInteger result;
    result = a;
    result /= b;
    return result;
}

LargeInteger &operator%=(LargeInteger &a, const LargeInteger &b) {
    if (isZero(b))
        throw("DIVISION_BY_ZERO");
    if (a < b) {
        return a;
    }
    if (a == b) {
        a = LargeInteger();
        return a;
    }
    int i, resultSize = 0, digit;
    int n = size(a), m = size(b);
    vector<int> result(n, 0);
    LargeInteger temp;
    for (i = n - 1; temp * 10 + a.data[i] < b; i--) {
        temp *= 10;
        temp += a.data[i];
    }
    for (; i >= 0; i--) {
        temp = temp * 10 + a.data[i];
        for (digit = 9; digit * b > temp; digit--);
        temp -= digit * b;
        result[resultSize++] = digit;
    }
    a = temp;
    return a;
}

LargeInteger operator%(const LargeInteger &a, const LargeInteger &b) {
    LargeInteger result;
    result = a;
    result %= b;
    return result;
}

LargeInteger &operator^=(LargeInteger &a, const LargeInteger &b) {
    LargeInteger exponent, base(a);
    exponent = b;
    a = 1;
    while (!isZero(exponent)) {
        if (exponent.data[0] & 1)
            a *= base;
        base *= base;
        halve(exponent);
    }
    return a;
}

LargeInteger operator^(LargeInteger &a, const LargeInteger &b) {
    LargeInteger result(a);
    result ^= b;
    return result;
}

void halve(LargeInteger &a) {
    int carry = 0;
    for (int i = a.data.size() - 1; i >= 0; i--) {
        int digit = (a.data[i] >> 1) + carry;
        carry = ((a.data[i] & 1) * 5);
        a.data[i] = digit;
    }
    while (a.data.size() > 1 && !a.data.back())
        a.data.pop_back();
}

LargeInteger computeSquareRoot(LargeInteger &a) {
    LargeInteger left(1), right(a), result(1), mid, product;
    halve(right);
    while (left <= right) {
        mid += left;
        mid += right;
        halve(mid);
        product = (mid * mid);
        if (product <= a) {
            result = mid;
            ++mid;
            left = mid;
        } else {
            --mid;
            right = mid;
        }
        mid = LargeInteger();
    }
    return result;
}

LargeInteger calculateCatalan(int n) {
    LargeInteger num(1), denom;
    for (int i = 2; i <= n; i++)
        num *= i;
    denom = num;
    for (int i = n + 1; i <= 2 * n; i++)
        denom *= i;
    num *= num;
    num *= (n + 1);
    denom /= num;
    return denom;
}

LargeInteger calculateFibonacci(int n) {
    LargeInteger a(1), b(1), c;
    if (!n)
        return c;
    n--;
    while (n--) {
        c = a + b;
        b = a;
        a = c;
    }
    return b;
}

LargeInteger calculateFactorial(int n) {
    LargeInteger result(1);
    for (int i = 2; i <= n; i++)
        result *= i;
    return result;
}

istream &operator>>(istream &in, LargeInteger &a) {
    string s;
    in >> s;
    int n = s.size();
    for (int i = n - 1; i >= 0; i--) {
        if (!isdigit(s[i]))
            throw("INVALID_INPUT");
        a.data[n - i - 1] = s[i];
    }
    return in;
}

ostream &operator<<(ostream &out, const LargeInteger &a) {
    for (int i = a.data.size() - 1; i >= 0; i--)
        cout << (short)a.data[i];
    return cout;
}

int main() {
    LargeInteger first("12345");
    cout << "Number of digits in first: " << size(first) << '\n';
    
    LargeInteger second(12345);
    if (first == second) {
        cout << "First and second are equal\n";
    } else {
        cout << "Not equal\n";
    }
    
    LargeInteger third("10000");
    LargeInteger fourth("100000");
    if (third < fourth) {
        cout << "Third is smaller than fourth\n";
    }
    
    LargeInteger fifth("10000000");
    if (fifth > fourth) {
        cout << "Fifth is larger than fourth\n";
    }

    cout << "first = " << first << '\n';
    cout << "second = " << second << '\n';
    cout << "third = " << third << '\n';
    cout << "fourth = " << fourth << '\n';
    cout << "fifth = " << fifth << '\n';

    first++;
    cout << "After incrementing first: " << first << '\n';
    
    LargeInteger sum = fourth + fifth;
    cout << "Sum of fourth and fifth = " << sum << '\n';
    
    LargeInteger product = second * third;
    cout << "Product of second and third = " << product << '\n';

    cout << "\n--------Fibonacci--------\n";
    for (int i = 0; i <= 30; i++) {
        LargeInteger fib = calculateFibonacci(i);
        cout << "Fibonacci " << i << " = " << fib << '\n';
    }
    
    cout << "\n--------Catalan--------\n";
    for (int i = 0; i <= 20; i++) {
        LargeInteger cat = calculateCatalan(i);
        cout << "Catalan " << i << " = " << cat << '\n';
    }

    cout << "\n--------Factorial--------\n";
    for (int i = 0; i <= 25; i++) {
        LargeInteger fact = calculateFactorial(i);
        cout << "Factorial " << i << " = " << fact << '\n';
    }
    
    return 0;
}