#pragma once

// 这个文件给出了一个栈的框架
// 栈是一种逻辑数据结构，它支持下面的3种操作：
// 1. 入栈
// 2. 出栈（保证后进先出，LIFO）
// 3. 访问栈顶元素

// 栈（包括队列、双端队列、优先队列等）的抽象层次很高
// 它规定了数据结构的功能，而非数据的组织形式
// 没有规定说栈必须要使用线性表实现，尽管现实中通常这样做
// 这一点在优先队列中尤其明显，尽管通常使用半线性结构表示优先队列，但也可以使用线性结构（牺牲性能）

#include "framework.h"

namespace clazy_framework {

// 栈的ADT
template <typename T>
class AbstractStack : public DataStructure<T> {
public:
    virtual void push(const T& e) = 0; // 将元素推入栈
    virtual T pop() = 0; // 弹出栈顶元素并返回
    virtual T& top() const = 0; // 获取栈顶元素
};

}