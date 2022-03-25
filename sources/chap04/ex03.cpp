#include "stack.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子展示了如何使用栈消除尾递归
// 对于没有返回值的函数，在函数尾部直接进行多次递归的情况
// 可以很方便地直接改写成使用栈而不使用递归的形式

template <typename T>
using Stack = clazy::Stack<T>;

// T表示递归函数的的参数
template <typename... T>
class TailRecursion : Algorithm {
protected:
    virtual void commonProcedure(T... args) = 0; // 不在递归边界时执行的操作
    virtual void borderProcedure(T... args) = 0; // 在递归边界上执行的操作
    virtual bool border(T... args) = 0; // 判断是否到达递归边界
    virtual vector<tuple<T...>> nextParameter(T... args) = 0; // 尾递归中的参数
public:
    virtual void recursiveFunction(T... args) { // 递归形式的函数
        if (border(args...)) {                  // 如果到达了递归边界
            borderProcedure(args...);           // 就执行递归边界的操作
        } else {
            commonProcedure(args...);           // 否则，执行不在递归边界的操作
            auto V = nextParameter(args...);
            for (auto x : V) { // 随后触发尾递归
                apply([&](T... y) { recursiveFunction(y...); }, x); // 这里用了tuple unpack的技术
            }
        }
    }
    virtual void interativeFunction(T... args) { // 迭代形式的函数
        Stack<tuple<T...>> S; // 用来存储
        S.push({args...});
        while (!S.empty()) {
            auto para = S.pop();
            if (apply([&](T... y) { return border(y...); }, para)) {
                apply([&](T... y) { borderProcedure(y...); }, para);
            } else {
                apply([&](T... y) { commonProcedure(y...); }, para);
                auto V = apply([&](T... y) { return nextParameter(y...); }, para);
                for (auto x : V | views::reverse) { // 注意，这个地方需要反序入栈，因为栈是LIFO的
                    S.push(x);
                }
            }
        }
    }
};

// 作为例子，考虑下面的一个问题
// 输入：无
// 输出：所有满足条件的8位数
//      它的8个数位均在1到8之间
//      输出顺序为从小到大的次序

class Generate4 : public Algorithm {
public:
    vector<int> ans;
    virtual void reset() { ans.clear(); }
    virtual void apply() = 0;
};

// 使用上面的尾递归模板解决上述问题
class Generate4Template : public TailRecursion<int> { // 输出结果到向量
protected:
    vector<int>& ans;
    virtual void commonProcedure(int x) override {} // 空
    virtual void borderProcedure(int x) override { ans.push_back(x); } // 查找到符合条件的解，写入到结果向量中
    virtual bool border(int x) override { return 10'000'000 <= x && x < 100'000'000; } // 递归边界：8位数
    virtual vector<tuple<int>> nextParameter(int x) override { // 每层递归增加一位
        vector<tuple<int>> V;
        for (int y : views::iota(1, 9)) {
            V.push_back({x*10 + y});
        }
        return V;
    }
public:
    Generate4Template(vector<int>& ans): ans(ans) {}
};

class Generate4TemplateRecursive : public Generate4 {
protected:
    Generate4Template gt;
public:
    virtual void apply() override { gt.recursiveFunction(0); }
    Generate4TemplateRecursive(): gt(this->ans) {}
};

class Generate4TemplateIterative : public Generate4 {
protected:
    Generate4Template gt;
public:
    virtual void apply() override { gt.interativeFunction(0); }
    Generate4TemplateIterative(): gt(this->ans) {}
};

// 作为对比，展示不使用上面的变换模板的，裸的求解函数的递归版本和迭代版本
class Generate4Recursive : public Generate4 {
protected:
    void solve(vector<int>& ans, int x) {
        if (10'000'000 <= x && x < 100'000'000) {
            ans.push_back(x);
        } else {
            for (int y : views::iota(1, 9)) {
                solve(ans, x*10 + y);
            }
        }
    }
public:
    virtual void apply() override{ solve(ans, 0); }
};

class Generate4Iterative : public Generate4 {
protected:
    void solve(vector<int>& ans, int x) {
        Stack<int> S; // ans是不变的，所以实际上不需要被加入到栈里
        S.push(x);
        while (!S.empty()) {
            x = S.pop();
            if (10'000'000 <= x && x < 100'000'000) {
                ans.push_back(x);
            } else {
                for (int y : views::iota(1, 9) | views::reverse) {
                    S.push(x*10 + y);
                }
            }
        }
    }
public:
    virtual void apply() override{ solve(ans, 0); }
};

int main() {
    auto algorithms = generateInstances<
        Generate4,
        Generate4TemplateRecursive,
        Generate4TemplateIterative,
        Generate4Recursive,
        Generate4Iterative
    >();
    applyTest<Generate4>(algorithms, [](auto algorithm) {
        algorithm->apply();
    });
    vector<int>& ans_ref = algorithms[0]->ans; // 下面确认4个算法的答案是一样的
    cout << "Answer size = " << ans_ref.size() << endl;
    for (auto algorithm : algorithms | views::drop(1)) {
        vector<int>& ans = algorithm->ans;
        assert(ans.size() == ans_ref.size()); // views::zip，可能要等C++23，现在只能一个一个比
        for (int i : views::iota(0, (int)ans.size())) {
            assert(ans[i] == ans_ref[i]);
        }
    }
    cout << "Answer Verified" << endl;
}
