#include "expression.hpp"

using namespace dslab;
using namespace std::literals::string_literals;

template <typename T = int>
using CalExpr = Algorithm<T(std::string_view)>;

template <typename T = int>
class CalExprSuffix : public CalExpr<T> {
public:
    T operator()(std::string_view expr) override {
        Expression<T> e { expr };
        e.infix2suffix();
        return e.calSuffix();
    }

    std::string type_name() const override {
        return "Calculate Expr (Suffix)";
    }
};

template <typename T = int>
class CalExprInfix : public CalExpr<T> {
public:
    T operator()(std::string_view expr) override {
        Expression<T> e { expr };
        return e.calInfix();
    }

    std::string type_name() const override {
        return "Calculate Expr (Infix)";
    }
};

std::vector<std::pair<std::string, int>> testData {
    { "(1+2^3*45)%67-8*9"s, -46 },
    { "-1-23*4^5-6!+7*8!/9"s, 7087 },
    { "(-1-(2+3*(45/6-7!))+8)*9"s, 135936 }
};

std::vector<std::size_t> testTimes { 1000, 10000, 100'000, 300'000 };

TestFramework<CalExpr<int>, CalExprSuffix<int>, CalExprInfix<int>> test;

int main() {
    for (const auto& [expr, result] : testData) {
        std::cout << std::format("Expression: {:>25}, Result: {:>7}", expr, result) << std::endl;
        test(expr);
    }
    for (auto n : testTimes) {
        std::cout << std::format("(1+(1+(1+..+(1+1))..), {:>7} times", n) << std::endl;
        std::string expr(4 * n - 3, '1');
        for (auto i { 0uz }; i < n - 1;  ++i) {
            expr[3 * i] = '(';
            expr[3 * i + 2] = '+';
            expr[4 * (n - 1) - i] = ')';
        }
        test(expr);
    }
    return 0;
}