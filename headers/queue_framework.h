#pragma once

#include "framework.h"

namespace clazy_framework {

// 这个文件给出的一个队列的框架
// 和栈一样，这里也没有继承线性表
template <typename T>
class AbstractQueue : public DataStructure<T> {
public:
    virtual T& front() const = 0; // 获取队列头元素
    virtual T dequeue() = 0; // 弹出队列头元素并返回
    virtual void enqueue(const T& e) = 0; // 将元素推入队列
};

}