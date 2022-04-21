#pragma once

// 这个文件给出了一个队列的框架
// 队列是一种逻辑数据结构，它支持下面的3种操作：
// 1. 入队
// 2. 出队（保证先进先出，FIFO）
// 3. 访问队首元素

#include "framework.h"

namespace clazy_framework {

// 队列的ADT
template <typename T>
class AbstractQueue : public DataStructure<T> {
public:
    virtual void enqueue(const T& e) = 0; // 将元素推入队列
    virtual T dequeue() = 0; // 弹出队首元素并返回
    virtual T& front() const = 0; // 获取队首元素
};

}