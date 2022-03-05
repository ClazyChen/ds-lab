// 问题1.8 - 幂
// 输入：正整数a、b
// 输出：a的b次幂

#include <iostream>
#include "power.h"
using namespace clazy_framework;

// 普通累乘幂
class PowerNaive : public PowerProblem {
protected:
    int power(int a, int b) const {
        int prod = 1;
        for (int i = 0; i < b; i++) {
            prod *= a;
        }
        return prod;
    }
};

// 快速幂（递归版本）
// 这个算法的思路非常容易理解，就是 a^b = a^(b/2)^2
class PowerRecursive : public PowerProblem {
protected:
    int power(int a, int b) const {
        if (b == 1) {        // 递归边界
            return a;        // 如果允许b = 0，则需要再写一个边界
        }
        int temp = power(a, b/2); // 计算a^(b/2)
        if (b % 2 == 1) {    // 注意因为整数除法是整除，这里需要区分奇偶性
            return temp * temp * a;
        } else /* b % 2 == 0 */ {
            return temp * temp;
        }
    }
};

// 快速幂（迭代版本）
using PowerIterative = clazy::Power;

int main() {
    auto algorithms = generateInstances<PowerProblem, PowerNaive, PowerRecursive, PowerIterative>();
    auto test = [&](int a, int b) {
        cout << "Testing a = " << a << " b = " << b << endl;
        applyTest<PowerProblem>(algorithms, [a, b](auto power) {
            cout << "answer = " << setw(11) << power->apply(a, b);
        });
    };
    pair<int, int> testData[] {
        {3, 4}, {2, 30}, {-3, 3}, // 一些常规问题
        {1, 1'000'000'000} // 当b很大时，效率差异明显
    };
    for (auto [a, b] : testData) {
        test(a, b);
    }
    return 0;
}