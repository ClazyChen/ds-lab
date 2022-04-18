#pragma once

// 这个文件给出了列表的基本框架
// 它继承于线性表，并需要实现列表的下列基本操作：
// 1. 任意位置的前插和后插
// 2. 删除任意位置的元素
// 3. 查找元素，返回位置

// 这个文件里定义了一个抽象的列表
// 并派生出了双向列表和单向列表
// 您在设计自己的数据结构时应当继承双向列表或单向列表的抽象类

#include "list_iterator_framework.h"

namespace clazy_framework {

// 列表的ADT接口
template <typename T>
class AbstractList : public AbstractLinearStructure<T, ListNodePos<T>> {
public:
    // 获取列表的规模、清空列表
    virtual int size() const = 0;
    virtual void clear() = 0;

    // 获取第一个和最后一个元素的位置
    virtual ListNodePos<T> getFrontPos() const = 0;
    virtual ListNodePos<T> getBackPos() const = 0;

    // 插入元素（包括前插和后插），返回新元素的位置
    virtual ListNodePos<T> insertAsPred(ListNodePos<T> pos, const T& e) = 0;
    virtual ListNodePos<T> insertAsSucc(ListNodePos<T> pos, const T& e) = 0;

    // 删除元素，返回被删除的元素
    virtual T remove(ListNodePos<T> pos) = 0;

    // 查找元素，返回位置，查找失败时返回无效位置
    virtual ListNodePos<T> find(const T& e) const = 0;

    // 实现基类的接口，注意这里有一些需要重写
    T& get(ListNodePos<T> pos) const override {
        return pos->data();
    }

    // 单向列表需要重写，因为不允许使用pred函数
    virtual ListNodePos<T> getPred(ListNodePos<T> pos) const override {
        return pos->pred();
    }

    ListNodePos<T> getSucc(ListNodePos<T> pos) const override {
        return pos->succ();
    }

    ListNodePos<T> invalidPos() const override {
        return nullptr;
    }

    // 在继承的时候，可能需要重写这2个函数
    // 因为这里是使用哨兵的写法，如果不使用哨兵的话，getBackPos()和getFrontPos()将在空表返回nullptr
    // 此时需要重载push_back()和push_front()
    virtual void push_back(const T& e) override {
        insertAsSucc(getBackPos(), e);
    }

    virtual void push_front(const T& e) override {
        insertAsPred(getFrontPos(), e);
    }

    T pop_back() override {
        return remove(getBackPos());
    }

    T pop_front() override {
        return remove(getFrontPos());
    }
};

// 对于单向和双向的列表，迭代器有所不同
template <typename T>
class AbstractBidirectionalList : public AbstractList<T> {
protected:
    using Iterator = ListIterator<T, AbstractBidirectionalList<T>>;
public:
    virtual Iterator begin() const {
        return Iterator(*this, getFrontPos());
    }

    virtual Iterator end() const {
        return Iterator(*this, getBackPos()->succ());
    }

    virtual Iterator rbegin() const {
        return Iterator(*this, getBackPos());
    }

    virtual Iterator rend() const {
        return Iterator(*this, getFrontPos()->pred());
    }
};

template <typename T>
class AbstractForwardList : public AbstractList<T> {
protected:
    using Iterator = ForwardListIterator<T, AbstractForwardList<T>>;
public:
    virtual Iterator begin() const {
        return Iterator(*this, getFrontPos());
    }

    virtual Iterator end() const {
        return Iterator(*this, getBackPos()->succ());
    }
};

}