#include <vector>
#include <format>
#include <iostream>
import Expression;
import Framework;
using namespace dslab;
using namespace std;

class CalExpr : public Algorithm<int(const string&)> {};

class CalExprSuffix : public CalExpr {
public:
    int operator()(const string& expr) override {
        Expression e { expr };
        e.infix2suffix();
        return e.calSuffix();
    }
};

class CalExprInfix : public CalExpr {
public:
    int operator()(const string& expr) override {
        Expression e { expr };
        return e.calInfix();
    }
};

vector<pair<string, int>> testData {
    {"(1+2^3*45)%67-8*9"s, -46},
    {"-1-23*4^5-6!+7*8!/9"s, 7087},
    {"(-1-(2+3*(45/6-7!))+8)*9"s, 135936}
};

vector<size_t> testTimes { 1000, 10000, 100'000, 300'000 };

TestFramework<CalExpr, CalExprSuffix, CalExprInfix> test;

int main() {
    for (const auto& [expr, result] : testData) {
        cout << format("Expression: {}, Result: {}", expr, result) << endl;
        test(expr);
    }
    for (auto n : testTimes) {
        cout << format("(1+(1+(1+..+(1+1))..), {} times", n) << endl;
        string expr(4 * n - 3, '1');
        for (size_t i { 0 }; i < n - 1; ++i) {
            expr[3 * i] = '(';
            expr[3 * i + 2] = '+';
            expr[4 * (n - 1) - i] = ')';
        }
        test(expr);
    }
    return 0;
}