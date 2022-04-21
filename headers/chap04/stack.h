#pragma once

#include "vector.h"
#include "stack_framework.h"

// 基于已有的线性表实现栈
// 默认的实现方式应该是使用向量的，但也允许您使用链表
// 从上一章的实验中可以看到，向量的效率比链表更高
// 在这里给出了一个栈的实现的例子，它接收的第二个模板参数允许用户传入链表
// 此外，也可以传入您自己设计的线性表数据结构
// 下面的实现中默认使用我的向量参考实现

namespace clazy {

template <typename T, typename Container = Vector<T>>
requires (clazy_framework::is_linear_structure<T, typename Container::Position, Container>)
class Stack : public clazy_framework::AbstractStack<T> {
private:
    Container C;
public:
    // 栈的三种基本操作
    void push(const T& e) override {
        C.push_back(e);
    }
    T pop() override {
        return C.pop_back();
    }
    T& top() const override {
        return C.back();
    }

    // 来自于通用数据结构类的接口
    void clear() override {
        C.clear();
    }
    int size() const override {
        return C.size();
    }

    // 输出方法声明为友元函数，以便输出栈内的具体内容
    template <typename T1, typename Container1>
    friend ostream& operator<<(ostream& out, const Stack<T1, Container1>& S);
};

template <typename T, typename Container>
ostream& operator<< (ostream& out, const Stack<T, Container>& S) {
    out << "Stack-";
    out << S.C;
    return out << "->";
}

}