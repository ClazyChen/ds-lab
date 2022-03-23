#include "stack.h"
#include "random.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子展示了括号匹配问题
// 这是一个经典的栈的应用问题

template <typename T>
using Stack = clazy::Stack<T>;

template <typename T>
using Vector = clazy::Vector<T>;

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

class ParenMatch : public Algorithm {
public:
    virtual bool apply(const Vector<char>& V) = 0;
    virtual bool apply(string str) {
        Vector<char> V;
        for (char c : str) {
            V.push_back(c);
        }
        return apply(V);
    }
};

// 使用栈进行括号匹配
class ParenMatchStack : public ParenMatch {
protected:
    Stack<char> S;
public:
    virtual bool apply(const Vector<char>& V) {
        S.clear(); // 初始化（清空栈）
        for (char c : V) {
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

int main() {
    shared_ptr<ParenMatch> matcher = make_shared<ParenMatchStack>();
    pair<string, bool> testCases[] { // 一些测试样例
        {"()()()()()()()", true},
        {"(((((())))))", true},
        {"(((((()))))))", false},
        {"((((((())))))", false},
        {"([{}])", true},
        {"([{]})", false}
    };
    for (auto [str, res] : testCases) { // 测试正确性
        cout << "Testing string = " << str << endl;
        bool match_res = matcher->apply(str);
        cout << "\tResult = " << match_res << endl;
        assert(match_res == res);
    }
    int testData[] { 10'000, 1'000'000, 10'000'000 };
    for (int n : testData) { // 测试时间性能
        cout << "Testing n = " << n << endl;
        auto V = randomStackOperation(n, '(', ')'); // 生成随机的括号序列
        applyTest<ParenMatch>(matcher, [&](auto algorithm) {
            cout << "answer = " << setw(5) << algorithm->apply(V);
        });
    }
    return 0;
}