#include "framework.h"
using namespace clazy_framework;

// 这个例子用来说明
// 相同的算法在实现上有所不同
// 会影响算法的可用性

// 问题：从1加到n
// 输入：正整数n
// 输出：1 + 2 + ... + n的和
class SumN : public Algorithm {
public:
    virtual int apply(int n) const = 0;
};

// 您可以实现自己的算法并参与比较
// class SumUser : public SumN {
// public:
//     int apply(int n) const {
//     // 在此处实现您自己的算法
//     }
// };

// 经典方法：直接利用等差数列求和公式计算
class SumNAP : public SumN {
public:
    int apply(int n) const override {
        return n * (n+1) / 2;
    }
};

// 改进方法：判断奇偶性，调换运算次序
class SumNAPM : public SumN {
public:
    int apply(int n) const override {
        if (n % 2 == 1) {
            return (n+1) / 2 * n;
        } else /* n % 2 == 0 */ {
            return n / 2 * (n+1);
        }
    }
};

// 再次改进的版本
// 如果计算结果溢出，不会返回一个异常值
// 而是会控制在INT_MAX
class SumNAPT : public SumN {
    const int threshold = 0x00010000;
public:
    int apply(int n) const override {
        if (n >= threshold) {
            return INT32_MAX;
        } else /* n < threshold */ {
            if (n % 2 == 1) {
                return (n+1) / 2 * n;
            } else /* n % 2 == 0 */ {
                return n / 2 * (n+1);
            }
        }
    }
};

int main() {
    auto algorithms = generateInstances<SumN, SumNAP, SumNAPM, SumNAPT>();
    auto test = [&](int n) {
        cout << "Testing n = " << n << endl;
        applyTest<SumN>(algorithms, [n](auto sum) {
            cout << "answer = " << setw(11) << sum->apply(n);
        });
    };
    int testData[] {  // 测试数据
        10,         // f1正确，f2正确，f3正确
        50000,      // f1溢出，f2正确，f3正确
        70000,      // f1溢出，f2溢出，f3输出上界
        -8          // 虽然从程序的角度，这样输入n是允许的，但从算法的角度，这样的输入没有意义。
    };
    for (int n : testData) {
        test(n);
    }

    // 像-8这样的值，如果无法正确处理这类输入，程序的稳健性可能是存在缺陷的。
    // 因为如果开放算法供其他程序使用，则无法保证调用者一定会给出有意义的值。
    // 如果要提升程序的稳健性，在输入非正数的n时，可以返回同样“无意义的值”，如0。
    // 但这并不是《数据结构》需要讨论的问题，因此以后也不会讨论。
    return 0;
}