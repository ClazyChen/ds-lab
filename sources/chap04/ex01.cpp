#include "stack.h"
using namespace clazy_framework;

// 这个例子展示了括号匹配问题
// 这是一个经典的栈的应用问题

template <typename T>
using Stack = clazy::Stack<T>;

template <typename T>
using Vector = clazy::Vector<T>;

// 在这个例子中，只处理左右小括号匹配的情况，不包含其他类型的括号
class ParenMatch : public Algorithm {
public:
    virtual bool apply(const Vector<char>& V) = 0;
};

// 使用栈进行括号匹配
class ParenMatchStack : public ParenMatch {
protected:
    Stack<char> S;
public:
    virtual bool apply(const Vector<char>& V) {
        S.clear();
        for (char c : V) {
            if (c == '(') {
                S.push(c);
            } else {
                if (S.empty()) {
                    return false;
                } else {
                    S.pop();
                }
            }
        }
        return S.empty(); // 如果完全匹配，最后剩余的是空栈
    }
};