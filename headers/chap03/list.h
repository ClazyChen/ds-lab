#pragma once

// 这个文件给出了双向列表的一个参考实现

#include "list_framework.h"

namespace clazy {

using clazy_framework::ListNode;
using clazy_framework::ListNodePos;

// 双向列表
template <typename T>
class List : public clazy_framework::AbstractBidirectionalList<T> {
protected:
    ListNodePos<T> _head;   // 列表的头哨兵
    ListNodePos<T> _tail;   // 列表的尾哨兵
    int _size;              // 列表的规模

public:
    List();
    List(const List& L);
    List(List&& L);
    ~List();
    List& operator=(const List& L);
    List& operator=(List&& L);

    int size() const override { return _size; }

    void clear() override;
    ListNodePos<T> getFrontPos() const override {
        return _head->succ();
    }
    ListNodePos<T> getBackPos() const override {
        return _tail->pred();
    }

    ListNodePos<T> insertAsPred(ListNodePos<T> pos, const T& e) override;
    ListNodePos<T> insertAsSucc(ListNodePos<T> pos, const T& e) override;
    T remove(ListNodePos<T> pos) override;
    ListNodePos<T> find(const T& e) const override;
};

// 构造函数、析构函数、赋值运算符重载
template <typename T>
List<T>::List() {
    _head = new ListNode<T>;
    _tail = new ListNode<T>;
    _head->setSucc(_tail);
    _tail->setPred(_head);
    _size = 0;
}

template <typename T>
List<T>::List(const List& L): List<T>() {
    for (auto& e : L) {
        push_back(e);
    }
}

template <typename T>
List<T>::List(List&& L): List<T>() {
    if (L._size > 0) {
        _head->setSucc(L._head->succ());
        _tail->setPred(L._tail->pred());
        _head->succ()->setPred(_head);
        _tail->pred()->setSucc(_tail);
        _size = L._size;
        L._size = 0;
    }
}

template <typename T>
List<T>::~List() {
    clear();
    delete _head;
    delete _tail;
}

template <typename T>
List<T>& List<T>::operator=(const List& L) {
    if (this != &L) {
        clear();
        for (auto& e : L) {
            push_back(e);
        }
    }
    return *this;
}

template <typename T>
List<T>& List<T>::operator=(List&& L) {
    if (this != &L) {
        clear();
        if (L._size > 0) {
            _head->setSucc(L._head->succ());
            _tail->setPred(L._tail->pred());
            _head->succ()->setPred(_head);
            _tail->pred()->setSucc(_tail);
            _size = L._size;
            L._size = 0;
        }
    }
    return *this;
}

// 清空列表中的所有元素
template <typename T>
void List<T>::clear() {
    auto p = getFrontPos();
    while (p != _tail) {
        auto q = p->succ();
        delete p;
        p = q;
    }
    _head->setSucc(_tail);
    _tail->setPred(_head);
    _size = 0;
}

// 插入元素，对于双向列表而言，前插和后插是对称的
template <typename T>
ListNodePos<T> List<T>::insertAsPred(ListNodePos<T> pos, const T& e) {
    auto p = new ListNode<T>(e);
    p->setSucc(pos);
    p->setPred(pos->pred());
    pos->pred()->setSucc(p);
    pos->setPred(p);
    _size++;
    return p;
}

template <typename T>
ListNodePos<T> List<T>::insertAsSucc(ListNodePos<T> pos, const T& e) {
    auto p = new ListNode<T>(e);
    p->setSucc(pos->succ());
    p->setPred(pos);
    pos->succ()->setPred(p);
    pos->setSucc(p);
    _size++;
    return p;
}

// 删除元素，返回被删除的元素
template <typename T>
T List<T>::remove(ListNodePos<T> pos) {
    auto p = pos->pred();
    auto q = pos->succ();
    p->setSucc(q);
    q->setPred(p);
    _size--;
    auto e = pos->_data;
    delete pos;
    return e;
}

// 查找元素，返回元素的位置，如果没有找到，返回空
template <typename T>
ListNodePos<T> List<T>::find(const T& e) const {
    auto p = getFrontPos();
    while (p != _tail && p->data() != e) {
        p = p->succ();
    }
    return p == _tail ? invalidPos() : p;
}

// 利用<<输出
template <typename T>
ostream& operator<< (ostream& out, const List<T>& L) {
    out << "L(";
    for (auto p = L.getFrontPos(); p != L.getBackPos(); p = p->succ()) {
        out << p->data();
        if (p->succ() != L.getBackPos()) {
            out << ", ";
        }
    }
    out << ")";
    return out;
}

}