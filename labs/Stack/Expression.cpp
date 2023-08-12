import Expression;
import Framework;
import std;

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
    string type_name() const override {
        return "Calculate Expr (Suffix)";
    }
};

class CalExprInfix : public CalExpr {
public:
    int operator()(const string& expr) override {
        Expression e { expr };
        return e.calInfix();
    }
    string type_name() const override {
        return "Calculate Expr (Infix)";
    }
};

class CalExprTree : public CalExpr {
public:
    int operator()(const string& expr) override {
        if (expr.size() > 100'000) {
            throw std::runtime_error("Stack overflow");
        }
        Expression e { expr };
        e.infix2suffix();
        return e.suffix2tree().cal();
    }
    string type_name() const override {
        return "Calculate Expr (Tree)";
    }
};

vector<pair<string, int>> testData {
    {"(1+2^3*45)%67-8*9"s, -46},
    {"-1-23*4^5-6!+7*8!/9"s, 7087},
    {"(-1-(2+3*(45/6-7!))+8)*9"s, 135936}
};

vector<size_t> testTimes { 1000, 10000, 100'000, 300'000 };

TestFramework<CalExpr, CalExprSuffix, CalExprInfix, CalExprTree> test;

int main() {
    for (const auto& [expr, result] : testData) {
        cout << format("Expression: {:>25}, Result: {:>7}", expr, result) << endl;
        test(expr);
    }
    for (auto n : testTimes) {
        cout << format("(1+(1+(1+..+(1+1))..), {:>7} times", n) << endl;
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