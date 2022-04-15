#include "power.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子对比了普通的累乘的求幂算法和快速幂

// 普通累乘幂
class PowerNaive : public PowerProblem {
public:
    int apply(int a, int b) override {
        int prod = 1;
        for (int i = 0; i < b; i++) {
            prod *= a;
        }
        return prod;
    }
};

// 快速幂（递归版本）
// 这个算法的思路非常容易理解，就是 a^b = (a^2)^(b/2)
class PowerRecursive : public PowerProblem {
public:
    int apply(int a, int b) override {
        if (b == 0) {
            return 1;
        }
        if (b % 2 == 0) {
            return apply(a * a, b / 2);
        } else {
            return a * apply(a * a, b / 2);
        }
    }
};

// 快速幂（迭代版本）
// 容易发现，这个算法和上面的递归版本是对应的
using PowerIterative = clazy::Power;

// 您可以自己尝试实现另一个思路
// 即a^b = (a^(b/2))^2 这个角度实现的快速幂
// 它和上面的迭代版本是不对应的，并且不容易改写成迭代版本

pair<int, int> testData[] {
    {3, 4}, {2, 30}, {-3, 3}, // 一些常规问题
    {1, 1'000'000'000} // 当b很大时，效率差异明显
};

int main() {
    TestFramework<PowerProblem, PowerNaive, PowerRecursive, PowerIterative> tf;
    for (auto [a, b] : testData) {
        cout << "Testing " << a << "^" << b << endl;
        tf.applyTest([a, b](auto algorithm) {
            cout << "answer = " << setw(11) << algorithm->apply(a, b);
        });
    }
    return 0;
}