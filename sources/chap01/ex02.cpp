#include "gcd.h"
using namespace clazy_framework;

// 这个例子展示了如何使用辗转相除法求最大公因数
// 您应当会证明它的正确性

// 求最大公因数
// 输入：正整数a、b
// 输出：a和b的最大公因数

// class GcdUser : public GcdProblem {
// protected:
//     int gcd(int a, int b) const {
//     // 在此处实现您自己的算法
//     }
// };

// 欧几里得算法（递归版本）
class GcdEuclidRecursive : public GcdProblem {
protected:
    int gcd(int a, int b) const override {
        if (b == 0) {
            return a;
        } else {
            return gcd(b, a % b);
        }
    }
};
// 上述版本在编译器优化之后，通常会消去递归
// 但是在《数据结构》中进行分析时，还是保留递归进行分析

// 欧几里得算法（迭代版本）
using GcdEuclidIterative = clazy::Gcd;

int main() {
    auto algorithms = generateInstances<GcdProblem, GcdEuclidRecursive, GcdEuclidIterative>();
    auto test = [&](int a, int b) {
        cout << "Testing a = " << a << " b = " << b << endl;
        applyTest<GcdProblem>(algorithms, [a, b](auto gcd) {
            cout << "answer = " << setw(4) << gcd->apply(a, b);
        });
    };
    pair<int, int> testData[] {
        {1, 1}, {0, 6}, {6, 0},
        {24, 18}, {720, 1024},
        {-8, 6},  // 未定义行为
        {8, -6}   // 未定义行为
    };
    for (auto [a, b] : testData) {
        test(a, b);
    }
    return 0;
}