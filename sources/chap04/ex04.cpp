#include "stack.h"
#include "combine.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这是另一个用栈消除尾递归的例子
// 在这个例子中需要依赖尾递归的返回值
// 因此不能像上面一样使用一个模板做转换

template <typename T>
using Stack = clazy::Stack<T>;

// 这个例子中展示的问题是组合数
// 众所周知，组合数C(n, m)可以用杨辉三角解决
// 递归方法
class CombineRecursive : public CombineProblem {
protected:
    virtual int apply(int n, int m) override {
        if (m == 0 || n == m) { // 递归边界
            return 1;
        } else {
            return apply(n-1, m-1) + apply(n-1, m); // 递归公式
        }
    }
};

// 使用栈的迭代方法
// 这个方法是上面的递归方法的直接改写
// 有了上一个实验的经验，您可以很容易地想象出，这个方法比递归方法更加低效
class CombineStack : public CombineProblem {
protected:
    virtual int apply(int n, int m) override {
        Stack<pair<int, int>> S; // 存储(n, m)
        int sum = 0;             // 目前为止累加的结果
        S.push({n, m});          // 初始的(n, m)
        while (!S.empty()) {
            tie(n, m) = S.pop();
            if (m == 0 || n == m) { // 递归边界
                sum++;           // 在递归边界上累加
            } else {
                S.push({n-1, m});
                S.push({n-1, m-1});
            }
        }
        return sum;
    }
};

// 经典的算法，比上述的算法快很多
// 上面的两个算法，时间复杂度都是O(C(m, n))的
// 而经典的递推方法可以达到O(m+n)的线性复杂度
using CombineBasic = clazy::Combine;

// 测试样例
pair<int, int> testData[] {
    {3, 2}, {6, 4}, {10, 5}, {20, 10}, {100, 4}, {50, 7}
};

int main() {
    TestFramework<CombineProblem, CombineRecursive, CombineStack, CombineBasic> tf;
    for (auto [m, n] : testData) {
        cout << "Testing n = " << n << " m = " << m << endl;
        tf.test(m, n);
    }
    return 0;
}
