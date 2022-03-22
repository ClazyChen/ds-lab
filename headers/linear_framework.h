#pragma once

#include "framework.h"
// 这个文件给出了一个线性表的框架
// 向量和列表共享这个框架

namespace clazy_framework {

// 线性表的ADT接口
template <typename T>
class AbstractLinearStructure : public DataStructure<T> {
public:
    // 其他接口，在向量和列表的基本框架中会被实现，不需要您自己定义
    virtual T& front() const = 0;
    virtual T& back() const = 0;
    virtual void push_front(const T&) = 0;
    virtual void push_back(const T&) = 0;
    virtual T pop_front() = 0;
    virtual T pop_back() = 0;
};

template <typename T, typename Container>
concept is_linear_structure = is_base_of_v<AbstractLinearStructure<T>, Container>;

}
