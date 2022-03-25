#include "framework.h"
using namespace clazy_framework;

// 这个例子从一个很简单的问题出发
// 展示基本的复杂度分析思路
// 输入：正整数n
// 输出：判断n是否为2的幂次

class Power2 : public Algorithm {
public:
    virtual bool test(int n) const = 0;
};

// 直接判定的方法，时间O(1)，空间O(1)
// 如果您熟悉整数的二进制存储方法，应该能证明这个算法的正确性
// 如果您不熟悉，可以在学习《组成原理》之后再思考这个问题
class Power2Directly : public Power2 {
public:
    bool test(int n) const override {
        return !(n & n-1);
    }
};

// 递归的方法，时间O(logn)，空间O(logn)
class Power2Recursive : public Power2 {
public:
    bool test(int n) const override {
        if (n % 2 == 1) {           // 奇数的情形
            return n == 1;          // 只有1是2的幂次
        } else /* n % 2 == 0 */ {   // 偶数的情形
            return test(n / 2);     // 当且仅当n/2是2的幂次时，n是2的幂次
        }
    }
};

// 注意，递归算法在输入n = 0时会陷入无限递归从而出错
// 尽管问题描述中规定n是正整数所以可以不考虑这个问题
// 但您可以作出思考，如何修改算法，可以避免这个问题发生？

// 循环的方法，时间O(logn)，空间O(logn)
// 这个循环方法的思想和上面的递归方法具有一些不同
// 区别1：它的迭代方向是“递推”而不是“递降”
// 区别2：递归边界和循环终止条件不对应
// 区别2是区别1的结果，更加本质，它指示了时间复杂度渐进下界不同的原因
class Power2Iterative : public Power2 {
public:
    bool test(int n) const override {
        int exp2 = 1;       // 找到第一个不小于n的2的幂次
        while (exp2 < n) {
            exp2 *= 2;
        }
        return exp2 == n;   // n是2的幂次，当且仅当找到的就是n
    }
};

// 上面的算法同样存在问题，您可以自己发现它，并进行修改

int main() {
    auto algorithms = generateInstances<Power2, Power2Directly, Power2Recursive, Power2Iterative>();
    auto test = [&](int n) {
        cout << "Testing n = " << n << endl;
        applyTest<Power2>(algorithms, [n](auto sum) {
            cout << "answer = " << setw(5) << sum->test(n);
        });
    };
    int testData[] {
        1, 2, 4, 6, 256, 65536,
        0x80000, 0x88888, 0x08'00'00'00 // 以上是几组常规数据
        // ,0x7f'ff'ff'ff // 较大的数据，但是会让上面的迭代版本陷入无限循环
        // ,0             // 无意义数据，但是会让上面的递归版本陷入无限递归
    };
    for (int n : testData) {
        test(n);
    }
    return 0;
}