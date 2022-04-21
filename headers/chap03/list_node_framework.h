#pragma once

// 这个文件给出了列表节点
// 在这个实现中，我们同时考虑单向链表和双向链表
// 并使用统一的接口控制它

#include "framework.h"

namespace clazy_framework {

// 动态链表的基类
template <typename T>
class AbstractListNode : public Object {
protected:
    T _data;
public:
    using P = AbstractListNode<T>*;

    // 直接前驱和直接后继的获取接口和设置接口
    virtual P pred() const = 0;
    virtual P succ() const = 0;
    virtual void setPred(P pred) = 0;
    virtual void setSucc(P succ) = 0;

    // 获取数据的接口
    T& data() { return _data; }

    // 默认实现的构造函数
    AbstractListNode() {}
    AbstractListNode(T data): _data(data) {}

    // 默认实现的比较接口
    bool operator==(const AbstractListNode<T>& other) const { return _data == other._data; }
    auto operator<=>(const AbstractListNode<T>& other) const = default;
};

// 动态列表使用指针表示位置
template <typename T>
using ListNodePos = AbstractListNode<T>::P;

// 单向（后向）节点
template <typename T>
class ForwardListNode : public AbstractListNode<T> {
protected:
    ListNodePos<T> _succ = nullptr;

    // 隐藏获取和设置前驱的接口
    ListNodePos<T> pred() const override { return nullptr; }
    void setPred(ListNodePos<T> pred) override {}

public:
    ForwardListNode() {}
    ForwardListNode(T data): AbstractListNode<T>(data) {}
    ListNodePos<T> succ() const override { return _succ; }
    void setSucc(ListNodePos<T> succ) override { _succ = succ; }
};

// 双向节点
template <typename T>
class ListNode : public ForwardListNode<T> {
protected:
    ListNodePos<T> _pred = nullptr;
public:
    ListNode() {}
    ListNode(T data): ForwardListNode<T>(data) {}
    ListNodePos<T> pred() const override { return _pred; }
    void setPred(ListNodePos<T> pred) override { _pred = pred; }
};

}
