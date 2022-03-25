#include "expression.h"
using namespace clazy_framework;

// 这个例子展示了如何使用栈来计算表达式

// 示例代码是实现的中缀表达式
using InfixExpression = clazy::InfixExpression;
using SuffixExpression = clazy::SuffixExpression;

// 计算一个表达式
class CalculationProblem : public Algorithm {
public:
    virtual int apply(string expr) = 0;
};

template <typename Expr>
requires (is_base_of_v<clazy_framework::AbstractExpression, Expr>)
class Calculation : public CalculationProblem {
protected:
    Expr expression;
public:
    virtual int apply(string expr) override {
        expression.parse(expr);
        return expression.getResult();
    }
};

int main() {
    auto algorithms = generateInstances<
        CalculationProblem,
        Calculation<InfixExpression>,
        Calculation<SuffixExpression>>();
    pair<string, int> testExpressions[] { // 一些示例表达式
        {"(1+2^3*45)%67-8*9", -46},
        {"-1-23*4^5-6!+7*8!/9", 7087},
        {"(-1-(2+3*(45/6-7!))+8)*9", 135936}
    };
    for (auto [expr, answer] : testExpressions) {
        cout << "Testing expression = " << setw(30) << expr << " which = " << setw(10) << answer << endl;
        applyTest<CalculationProblem>(algorithms, [expr, answer](auto calc) {
            cout << "answer = " << setw(10) << calc->apply(expr);
        });
    }
}
