#include "gcd.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示了如何使用辗转相除法求最大公因数
// 此算法常常用来作为基本算法的一个举例
// 您应当会证明它的正确性

// 求最大公因数
// 输入：正整数a、b
// 输出：a和b的最大公因数

// 欧几里得算法（递归版本）
class GcdEuclidRecursive : public GcdProblem {
public:
    int apply(int a, int b) override {
        if (b == 0) {
            return a;
        }
        return apply(b, a % b);
    }
};

// 上述版本在编译器优化之后，通常会消去递归
// 但是在《数据结构》中进行分析时，还是保留递归进行分析

// 欧几里得算法（迭代版本）
// 为了减少重复性，在头文件的clazy命名空间写的代码，在源文件中使用别名
// 您可以跳转到对应头文件里去查看
using GcdEuclidIterative = clazy::Gcd;

// 测试数据
pair<int, int> testData[] {
    {1, 1}, {1, 6}, 
    { 100'000'008, 12'345'678 },
    {24, 18}, {720, 1024},
    {0, 0},   // 未定义行为
    {0, 6},   // 未定义行为
    {-8, 6},  // 未定义行为
    {6, -8},  // 未定义行为
    {-8, -8}, // 未定义行为
    {-8, 4},  // 未定义行为
    {8, -6}   // 未定义行为
};

// 当我们设计算法的时候，假定输入数据a和b都是正整数
// 所以当我们输入负数或者0时，实际上是未定义行为
// 此时返回的结果是无法控制的，比如一正一负的情况下，结果的符号是不确定的
// 如果您追求更好的健壮性，可以为非正数的情况下补充合理的定义

int main() {
    TestFramework<GcdProblem, GcdEuclidRecursive, GcdEuclidIterative> tf;
    for (auto [a, b] : testData) {
        cout << "Testing a = " << a << " b = " << b << endl;
        tf.test(a, b);
    }
    return 0;
}