#pragma once

// 这个文件给出了单向列表的一个参考实现
// 大体上看，它和双向列表的实现是非常接近的
// 我也推荐您想办法把它们合并到一个类里
// 我使用了统一的节点类，就是做了这样的一步工作
// 不过在这里，我把它们拆开了，为了看得更加清晰

#include "list_framework.h"

namespace clazy {

using clazy_framework::ForwardListNode;
using clazy_framework::ListNodePos;

// 单向列表
template <typename T>
class ForwardList : public clazy_framework::AbstractForwardList<T> {
protected:
    ListNodePos<T> _head; // 列表的头哨兵
    ListNodePos<T> _tail; // 列表的尾哨兵
    int _size;            // 列表的规模

public:
    ForwardList();
    ForwardList(const ForwardList& L);
    ForwardList(ForwardList&& L);
    ~ForwardList();
    auto& operator=(const ForwardList& L);
    auto& operator=(ForwardList&& L);

    int size() const override { return _size; }

    void clear() override;
    ListNodePos<T> getFrontPos() const override { 
        return _head->succ(); 
    }

    // 单向链表的这两个指针需要重写，它们需要从前向后遍历才能找到
    ListNodePos<T> getBackPos() const override {
        return getPrev(_tail);
    }
    ListNodePos<T> getPred(ListNodePos<T> pos) const override;

    ListNodePos<T> insertAsPred(ListNodePos<T> pos, const T& e) override;
    ListNodePos<T> insertAsSucc(ListNodePos<T> pos, const T& e) override;
    T remove(ListNodePos<T> pos) override;
    ListNodePos<T> find(const T& e) const override;
};

// 构造函数、析构函数、赋值运算符重载
template <typename T>
ForwardList<T>::ForwardList() {
    _head = new ForwardListNode<T>;
    _tail = new ForwardListNode<T>;
    _head->setSucc(_tail);
    _size = 0;
}

template <typename T>
ForwardList<T>::ForwardList(const ForwardList& L): ForwardList<T>() {
    for (auto& e : L) {
        push_back(e);
    }
}

template <typename T>
ForwardList<T>::ForwardList(ForwardList&& L): ForwardList<T>() {
    if (L._size > 0) {
        _head = L._head;
        _tail = L._tail;
        _size = L._size;
        L._head = L._tail = nullptr;
        L._size = 0;
    }
}

// 因为在移动构造和移动赋值的时候，有可能把原先的头、尾哨兵置为空
// 所以析构的时候需要特别判断
template <typename T>
ForwardList<T>::~ForwardList() {
    clear();
    if (_head != nullptr) {
        delete _head;
        _head = nullptr;
    }
    if (_tail != nullptr) {
        delete _tail;
        _tail = nullptr;
    }
}

template <typename T>
auto& ForwardList<T>::operator=(const ForwardList& L) {
    if (this != &L) {
        clear();
        for (auto& e : L) {
            push_back(e);
        }
    }
    return *this;
}

template <typename T>
auto& ForwardList<T>::operator=(ForwardList&& L) {
    if (this != &L) {
        clear();
        if (L._size > 0) {
            _head = L._head;
            _tail = L._tail;
            _size = L._size;
            L._head = L._tail = nullptr;
            L._size = 0;
        }
    }
    return *this;
}

// 清空列表中的所有元素，这里和双向是一致的
template <typename T>
void ForwardList<T>::clear() {
    auto p = getFrontPos();
    while (p != invalidPos()) {
        auto q = p->succ();
        delete p;
        p = q;
    }
    _head->setSucc(nullptr);
    _size = 0;
}

// 插入元素，这里前插和后插是不对称的
// 前插实际上是通过后插实现的，这里将会导致潜在的危险：原先指向pos的指针将会指向新插入的元素
template <typename T>
ListNodePos<T> ForwardList<T>::insertAsPred(ListNodePos<T> pos, const T& e) {
    auto p = insertAsSucc(pos, pos->data());
    p->data() = e;
    return p;
}

// 后插则和双向列表是一致的
template <typename T>
ListNodePos<T> ForwardList<T>::insertAsSucc(ListNodePos<T> pos, const T& e) {
    auto p = new ForwardListNode<T>(e);
    p->setSucc(pos->succ());
    pos->setSucc(p);
    _size++;
    return p;
}

// 删除元素，这里面临的问题和前插一样，也同样会导致潜在的危险
// 引入尾哨兵的目的就是为了能够删除最后一个元素
template <typename T>
T ForwardList<T>::remove(ListNodePos<T> pos) {
    auto p = pos->succ();
    pos->setSucc(p->succ());
    _size--;
    auto e = p->data();
    delete p;
    return e;
}

// 查找元素，和双向列表一样
template <typename T>
ListNodePos<T> ForwardList<T>::find(const T& e) const {
    auto p = getFrontPos();
    while (p != invalidPos() && p->data() != e) {
        p = p->succ();
    }
    return p;
}

// 重写获取前一个元素的方法，需要从头开始遍历
template <typename T>
ListNodePos<T> ForwardList<T>::getPred(ListNodePos<T> pos) const {
    auto p = _head;
    while (p->succ() != pos) {
        p = p->succ();
    }
    return p;
}

// 利用<<输出
template <typename T>
ostream& operator<< (ostream& out, const ForwardList<T>& L) {
    out << "FL(";
    for (auto p = L.getFrontPos(); p->succ()->succ() != L.invalidPos(); p = p->succ()) {
        out << p->data();
        if (p->succ()->succ()->succ() != L.invalidPos()) {
            out << ", ";
        }
    }
    out << ")";
    return out;
}

}