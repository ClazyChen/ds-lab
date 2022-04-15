#pragma once

// 这个文件给出了向量的基本框架
// 它继承于线性表，并需要实现向量的下列基本操作：
// 1. 任意位置的插入
// 2. 任意位置的删除
// 3. 查找元素，返回位置

// 对于向量来说，元素的位置是一个整数，从0开始
// 在邓俊辉《数据结构》里被称为“秩”，即Rank

// 实验指南：
// 在《数据结构》中，您可以自己实现数据结构，也可以使用我提供的数据结构，在上面进行操作
// 比如，您可以自己实现一个向量，继承于这个文件里给定的向量模板
// 其中，需要您实现的函数就是在这个抽象基类里定义的纯虚函数
// 然后用您自己的数据结构去完成各个lab，验证您的实现的正确性
// 初学者也可以不实现自己的数据结构，在我提供的示例上进行操作

#include "vector_iterator_framework.h"

namespace clazy_framework {

// 向量的ADT接口
template <typename T>
class AbstractVector : public AbstractLinearStructure<T, Rank> {
protected:
    // 使用的迭代器
    using Iterator = VectorIterator<T, AbstractVector<T>>;
public:
    // 获取向量的容量、规模、数据区域的首地址
    virtual int capacity() const = 0;
    virtual int size() const = 0;
    virtual T* data() const = 0;

    // 修改容量和规模
    virtual void reserve(int capacity) = 0;
    virtual void resize(int size) = 0;

    // 插入元素，返回被插入元素的秩
    virtual Rank insert(Rank pos, const T& e) = 0;

    // 删除元素，返回被删除的元素
    virtual T remove(Rank pos) = 0;

    // 查找元素，返回元素的秩，如果没有找到，返回-1
    virtual Rank find(const T& e) const = 0;

    // 实现基类的接口
    bool empty() const override {
        return size() == 0;
    }

    void clear() override {
        resize(0);
    }

    T& get(Rank pos) const override {
        return data()[pos];
    } 

    Rank getPrev(Rank pos) const override {
        return pos - 1;
    }

    Rank getNext(Rank pos) const override {
        return pos + 1;
    }

    Rank getFrontPos() const {
        return 0;
    }

    Rank getBackPos() const {
        return size() - 1;
    }

    void push_back(const T& e) override {
        insert(getBackPos() + 1, e);
    }

    void push_front(const T& e) override {
        insert(getFrontPos(), e);
    }

    T pop_back() override {
        return remove(getBackPos());
    }

    T pop_front() override {
        return remove(getFrontPos());
    }

    // 用于迭代器的接口
    virtual Iterator begin() const {
        return Iterator(*this, getFrontPos());
    }

    virtual Iterator end() const {
        return Iterator(*this, getBackPos() + 1);
    }

    virtual Iterator rbegin() const {
        return Iterator(*this, getBackPos());
    }

    virtual Iterator rend() const {
        return Iterator(*this, getFrontPos() - 1);
    }
};


}
