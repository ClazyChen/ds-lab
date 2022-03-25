#include <numeric>
#include "framework.h"
#include "random.h"
using namespace clazy_framework;

// 这个例子展示了一个分治算法
// 您应当会证明它的正确性

// 数组求和
// 输入：数组A[0:n]
// 输出：A[0]+A[1]+...+A[n-1]的和

class SumA : public Algorithm {
public:
    virtual int apply(const int* A, int n) const = 0;
};

// 采用分治算法进行的求和
class SumADnC : public SumA {
public:
    int apply(const int* A, int n) const override {
        if (n == 0) { return 0; }
        if (n == 1) { return A[0]; }
        return apply(A, n/2) + apply(A + n/2, n - n/2);
    }
};

// 普通方法求和
// 用于对照试验，测试上述分治算法的正确性
class SumAstd : public SumA {
public:
    int apply(const int* A, int n) const override {
        return accumulate(A, A+n, 0);
    }
};

int main() {
    auto algorithms = generateInstances<SumA, SumADnC, SumAstd>();
    auto test = [&](int n) {
        cout << "Testing n = " << n << endl;
        auto data = randomVector(n);
        applyTest<SumA>(algorithms, [&](auto sum) {
            cout << "answer = " << setw(11) << sum->apply(&(data[0]), n);
        });
    };
    // 测试数据可能造成溢出，溢出情况下可以观察到，两种算法仍然能返回一样的值
    // 但是可以观测到，分治算法的时间在n很大时，显著高于常规求和算法
    // 您可以利用之后的知识理解这一点
    int testSize[] { 0, 1, 5, 100, 10000, 1'000'000, 100'000'000 };
    for (int n : testSize) {
        test(n);
    }
    return 0;
}