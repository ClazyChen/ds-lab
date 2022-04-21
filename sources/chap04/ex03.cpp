#include "stack.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示了如何使用栈消除尾递归
// 对于没有返回值的函数，在函数尾部直接进行多次递归的情况
// 可以很方便地直接改写成使用栈而不使用递归的形式

template <typename T>
using Stack = clazy::Stack<T>;

// Args...表示递归函数的参数
// 内部使用的apply是std::apply，用于执行tuple unpack
// 这里提供了一个通用的尾递归转换方法，不过可读性可能不太好
// 后面将通过一个具体的例子展示
template <typename... Args>
class TailRecursion : Object {
protected:
    virtual void commonProcedure(Args... args) = 0; // 不在递归边界时执行的操作
    virtual void borderProcedure(Args... args) = 0; // 在递归边界上执行的操作
    virtual bool border(Args... args) = 0; // 判断是否到达递归边界
    virtual vector<tuple<Args...>> nextParameter(Args... args) = 0; // 尾递归中的参数
public:
    virtual void recursiveFunction(Args... args) { // 递归形式的函数
        if (border(args...)) {                  // 如果到达了递归边界
            borderProcedure(args...);           // 就执行递归边界的操作
        } else {
            commonProcedure(args...);           // 否则，执行不在递归边界的操作
            auto V = nextParameter(args...);
            for (auto&& x : V) { // 随后触发尾递归
                apply([&](Args... y) { recursiveFunction(y...); }, x); // 这里用了tuple unpack的技术
            }
        }
    }
    virtual void interativeFunction(Args... args) { // 迭代形式的函数
        Stack<tuple<Args...>> S; // 用来存储
        S.push({args...});
        while (!S.empty()) {
            auto para = S.pop();
            if (apply([&](Args... y) { return border(y...); }, para)) {
                apply([&](Args... y) { borderProcedure(y...); }, para);
            } else {
                apply([&](Args... y) { commonProcedure(y...); }, para);
                auto V = apply([&](Args... y) { return nextParameter(y...); }, para);
                for (auto&& x : V | views::reverse) { // 注意，这个地方需要反序入栈，因为栈是LIFO的
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

// 我们通过4种方法来解决这个问题
// 1. 使用上述模板的尾递归版本
// 2. 使用上述模板的迭代版本
// 3. 不使用上述模板的尾递归版本
// 4. 不使用上述模板的迭代版本

// 其中，不使用模板的版本和使用模板的版本在语义上是一样的
// 作为一个例子，展示如何从上述模板的角度去解决这个尾递归和迭代版本相互转换的问题
// 最后，我们验证这4种方法生成的结果是一致的

class Generate4 : public Algorithm<> {
public:
    vector<int> ans;
    virtual void clear() override { 
        ans.clear();
    }
};

// 使用上面的尾递归模板解决上述问题
// 下面是这个问题对应的尾递归模板
class Generate4Template : public TailRecursion<int> {
protected:
    vector<int>& ans;
    virtual void commonProcedure(int x) override {} // 空
    virtual void borderProcedure(int x) override { ans.push_back(x); } // 查找到符合条件的解，写入到结果向量中
    virtual bool border(int x) override { return 10'000'000 <= x && x < 100'000'000; } // 递归边界：8位数
    virtual vector<tuple<int>> nextParameter(int x) override { // 每层递归增加一位
        vector<tuple<int>> V;
        for (int y = 1; y <= 8; y++) {
            V.push_back({x*10 + y});
        }
        return V;
    }
public:
    Generate4Template(vector<int>& ans): ans(ans) {}
};

// 基于模板的两种方法
class Generate4TemplateRecursive : public Generate4 {
protected:
    Generate4Template gt;
public:
    virtual void apply() override { 
        gt.recursiveFunction(0); 
    }
    Generate4TemplateRecursive(): gt(this->ans) {}
};

class Generate4TemplateIterative : public Generate4 {
protected:
    Generate4Template gt;
public:
    virtual void apply() override { 
        gt.interativeFunction(0);
    }
    Generate4TemplateIterative(): gt(this->ans) {}
};

// 作为对比，展示不使用上面的变换模板的，裸的求解函数的递归版本和迭代版本
class Generate4Recursive : public Generate4 {
protected:
    void solve(vector<int>& ans, int x) {
        if (10'000'000 <= x && x < 100'000'000) {
            ans.push_back(x);
        } else {
            for (int y = 1; y <= 8; y++) {
                solve(ans, x*10 + y);
            }
        }
    }
public:
    virtual void apply() override {
        solve(ans, 0);
    }
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
                for (int y = 8; y >= 1; y--) {
                    S.push(x*10 + y);
                }
            }
        }
    }
public:
    virtual void apply() override{ 
        solve(ans, 0);
    }
};

int main() {
    TestFramework<
        Generate4,
        Generate4TemplateRecursive,
        Generate4TemplateIterative,
        Generate4Recursive,
        Generate4Iterative
    > tf;
    tf.clear();
    tf.test();
    // 下面验证4种方法生成的结果是否一致
    auto&& ans_ref = tf.instances[0]->ans;
    cout << "Answer size = " << ans_ref.size() << endl;
    for (auto instance : tf.instances | views::drop(1)) {
        auto&& ans = instance->ans;
        assert(ans.size() == ans_ref.size()); // views::zip，可能要等C++23，现在只能一个一个比
        for (int i = 0; i < ans.size(); i++) {
            assert(ans[i] == ans_ref[i]);
        }
    }
    cout << "Answer Verified" << endl;
}

// 模板里面有很多对于这个问题而言不必要的封装，从而拉低了代码的效率
// 另一方面，您可以发现，尾递归的运行时间，实际上很有可能比相应的迭代版本更高
// 这得益于编译器优化和底层实现。因此，在机试中并不需要刻意地将尾递归改写成迭代方法
// 挑战编译器的智力，对绝大多数编程人员来说都是极具浪漫主义色彩的冒险行为
