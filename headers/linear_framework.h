#pragma once

// 这个文件给出了一个线性表的框架
// 向量和列表共享这个框架

namespace clazy_framework {

// 线性表的ADT接口
template <typename T>
class AbstractLinearStructure {
public:
    // 您需要允许用户清空数据结构
    virtual void clear() = 0;

    // 其他接口，在向量和列表的基本框架中会被实现，不需要您自己定义
    virtual bool empty() const = 0;
    virtual T& front() const = 0;
    virtual T& back() const = 0;
    virtual void push_front(const T&) = 0;
    virtual void push_back(const T&) = 0;
    virtual T pop_front() = 0;
    virtual T pop_back() = 0;
};

}
