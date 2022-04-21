#include "stack.h"
#include "random.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示了括号匹配问题
// 这是一个经典的栈的应用问题

template <typename T>
using Stack = clazy::Stack<T>;

// 在这个例子中，处理三种类型的括号匹配情况

// 用来从右括号映射到左括号，如果是左括号则映射到自身
// 如果是非括号的字符，则映射到0
char getLeft(char c) {
    switch (c) {
        case '(': case ')': return '(';
        case '[': case ']': return '[';
        case '{': case '}': return '{';
        default: return '\0';
    }
}

// 实验分为两部分：
// 1. 对给定的测试数据进行验证
// 2. 对随机生成的超大规模数据进行时间测试

class ParenMatch : public Algorithm<bool, const vector<char>&>{
public:
    virtual void check(string str, bool result) {
        vector<char> v;
        for (char c : str) {
            v.push_back(c);
        }
        assert(apply(v) == result);
    }
};

// 使用栈进行括号匹配
class ParenMatchStack : public ParenMatch {
protected:
    Stack<char> S;
public:
    virtual bool apply(const vector<char>& v) override {
        S.clear(); // 初始化（清空栈）
        for (char c : v) {
            if (char left = getLeft(c); left == c) { // 如果c是左括号
                S.push(c);                           // 则将c入栈
            } else {
                if (S.empty() || left != S.pop()) {  // 如果c是右括号
                    return false;                    // 并且S为空或者栈顶不匹配
                }                                    // 则说明括号匹配错误
            }
        }
        return S.empty(); // 如果完全匹配，最后剩余的是空栈
    }
};

// 验证正确性的测试样例
pair<string, bool> testCases[] {
    {"()()()()()()()", true},
    {"(((((())))))", true},
    {"(((((()))))))", false},
    {"((((((())))))", false},
    {"([{}])", true},
    {"([{]})", false}
};

// 计算时间的测试样例
// 这里生成的必定是合法的括号序列，所以测试结果必定为1
int testData[] { 10, 10'000, 1'000'000, 10'000'000 };

int main() {
    TestFramework<ParenMatchStack> tf;  
    for (auto [str, res] : testCases) { // 测试正确性
        tf.invoke(bind(&ParenMatch::check, placeholders::_1, str, res));
    }
    for (int n : testData) { // 测试时间性能
        cout << "Testing n = " << n << endl;
        auto v = clazy::RandomStackOperation<char>()(n, '(', ')'); // 生成随机的括号序列
        tf.test(v);
    }
    return 0;
}
