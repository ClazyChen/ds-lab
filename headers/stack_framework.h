#pragma once

#include "linear_framework.h"

namespace clazy_framework {

// 这个文件给出了一个栈的框架
// 这里没有继承线性表，因为线性表实际上提供的双端队列的接口
template <typename T>
class AbstractStack : public DataStructure<T> {
public:
    virtual T& top() const = 0; // 获取栈顶元素
    virtual T pop() = 0; // 弹出栈顶元素并返回
    virtual void push(const T& e) = 0; // 将元素推入栈
};

}