#include "random.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示了一个分治算法
// 您应当会证明它的正确性

// 数组求和
// 输入：数组A[0:n]
// 输出：A[0]+A[1]+...+A[n-1]的和

// 第一个参数表示数组A的首地址
// 第二个参数表示数组A的长度（n > 0）
class SumA : public Algorithm<int, const int*, int> {

};

// 采用分治算法进行的求和
class SumADnC : public SumA {
public:
    int apply(const int* A, int n) override {
        if (n == 1) {
            return A[0];
        }
        int m = n / 2;
        return apply(A, m) + apply(A + m, n - m);
    }
};

// 普通方法求和
// 用于对照试验，测试上述分治算法的正确性
// 如果您不了解accumulate函数，请参考C++标准库
class SumAstd : public SumA {
public:
    int apply(const int* A, int n) override {
        return accumulate(A, A+n, 0);
    }
};

// 测试数据可能造成溢出，溢出情况下可以观察到，两种算法仍然能返回一样的值
// 但是可以观测到，分治算法的时间消耗，在n很大时，显著高于常规求和算法
// 您可以利用之后的知识理解这一点
int testSize[] { 1, 5, 100, 10000, 1'000'000, 100'000'000 };

int main() {
    TestFramework<SumA, SumADnC, SumAstd> tf;
    for (int n : testSize) {
        cout << "Testing n = " << n << endl;
        auto data = clazy::RandomVector()(n);
        tf.test(data.data(), n);
    }
    return 0;
}
