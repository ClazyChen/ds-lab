#include "expression_suffix.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示了如何使用栈来计算表达式

using InfixExpression = clazy::InfixExpression;
using SuffixExpression = clazy::SuffixExpression;

pair<string, int> testExpressions[] { // 一些示例表达式
    {"(1+2^3*45)%67-8*9", -46},
    {"-1-23*4^5-6!+7*8!/9", 7087},
    {"(-1-(2+3*(45/6-7!))+8)*9", 135936}
};

int main() {
    TestFramework<AbstractExpression, InfixExpression, SuffixExpression> tf;
    for (auto [expr, answer] : testExpressions) {
        cout << "Testing expression = " << setw(30) << expr << " which = " << setw(10) << answer << endl;
        tf.test(expr);
    }
}
