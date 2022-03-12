#pragma once

#include "list.h"
#include "vector.h"

namespace clazy {

// 这个文件引入静态链表
// 它的实现思路和动态链表有所不同
// 是基于一个向量的

// 这里使用的静态链表，是基于指针之差进行设计的
// 因为向量扩容的时候，所有节点的地址都会发生变化

// 在静态链表实现中，采用特殊值表示指向空位置
static constexpr ptrdiff_t nullDiff = INT32_MAX;

// 单向静态节点
template <typename T>
class ForwardStaticListNode : public AbstractListNode<T> {
protected:
    T _data;
    ptrdiff_t _succ = nullDiff;
public:
    ForwardStaticListNode() {}
    ForwardStaticListNode(T _data): _data(_data) {}
    virtual ListNodePos<T> setSucc(ListNodePos<T> _succ) {
        if (_succ == nullptr) {
            this->_succ = nullDiff;
        } else {
            this->_succ = _succ - this; 
        }
        return this; 
    }
    virtual ListNodePos<T> succ() { 
        if (_succ == nullDiff) {
            return nullptr;
        } else {
            return this + _succ;
        }    
    }
    virtual T& data() { return _data; }
    constexpr static bool isBidirectional() { return false; }
};

// 双向静态节点
template <typename T>
class StaticListNode : public AbstractListNode<T> {
protected:
    ptrdiff_t _pred = nullDiff;
public:
    StaticListNode() {}
    StaticListNode(T _data): ForwardStaticListNode<T>(_data) {}
    virtual ListNodePos<T> setPred(ListNodePos<T> _pred) {
        if (_pred == nullptr) {
            this->_pred = nullDiff;
        } else {
            this->_pred = _pred - this; 
        }
        return this; 
    }
    virtual ListNodePos<T> pred() { 
        if (_pred == nullDiff) {
            return nullptr;
        } else {
            return this + _pred;
        }    
    }
    constexpr static bool isBidirectional() { return true; }
};

// 静态列表
template <typename T, typename Node = ListNode<T>, bool Circular = false, typename Container = Vector<Node>>
requires (is_base_of_v<clazy_framework::AbstractListNode<T>, Node> && is_base_of_v<clazy_framework::AbstractVector<Node>, Container>)
class StaticList : public List<T, Node, Circular> {
protected:
    Container V;            // 在静态列表中，用向量维护

    // 重载创建新节点的接口，改为在向量中加入一个新元素
    virtual ListNodePos<T> create() const {
        V.push_back(Node());
        return (end(V) - 1).base();
    }
    virtual ListNodePos<T> create(const T& e) const {
        V.push_back(Node(e));
        return (end(V) - 1).base();
    }
    
    // 重载删除节点的接口，改为将节点交换到向量末尾并释放
    virtual void destroy(ListNodePos<T> pos);
    // 重载删除所有节点的接口，什么都不用做
    virtual void destroyAll() {}

public:
    StaticList(): List<T, Node, Circular>() {}
    StaticList(const StaticList<T, Node, Circular, Container>& L): List<T, Node, Circular>(L) {}
};

// 删除元素，列表删除 + 向量删除
template <typename T, typename Node, bool Circular, typename Container>
void StaticList<T, Node, Circular, Container>::destroy(ListNodePos<T> pos) {
    ListNodePos<T> last = (end(V) - 1).base();  // 再执行向量删除
    if (pos != last) {                          // 如果pos不在末尾，将末尾的元素移动到pos处
        ListNodePos<T> pred = last->pred(), succ = last->succ();
        pos->data() = last->data();
        pos->setPred(pred->setSucc(pos))->setSucc(succ->setPred(pos)); // 四次赋值
    }
    V.pop_back();
}

}