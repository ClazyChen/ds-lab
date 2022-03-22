#pragma once

#include "vector.h"
#include "list_basic.h"
#include "stack.h"

// 这个文件基于基本列表实现了静态链表

namespace clazy {

const static int invalidRank = -1;

// 静态列表节点的抽象基类
template <typename T>
class AbstractStaticListNode {
protected:
    T _data;
public:
    // 您需要实现它的直接前驱和直接后继的获取接口和设置接口（返回本身的指针）
    virtual Rank pred() { return invalidRank; }
    virtual Rank succ() { return invalidRank; }
    virtual void setPred(Rank _pred) { }
    virtual void setSucc(Rank _succ) { }

    // 获取数据的接口
    virtual T& data() { return _data; }

    // 默认实现的构造函数
    AbstractStaticListNode() {}
    AbstractStaticListNode(T _data): _data(_data) {}

    // 默认实现的比较接口
    bool operator==(const AbstractStaticListNode<T>& other) const { return _data == other._data; };
    bool operator!=(const AbstractStaticListNode<T>& other) const { return !(*this == other);}
};

// 单向（后向）节点
template <typename T>
class ForwardStaticListNode : public AbstractStaticListNode<T> {
protected:
    Rank _succ = invalidRank;
public:
    ForwardStaticListNode() {}
    ForwardStaticListNode(T _data): AbstractStaticListNode<T>(_data) {}
    virtual Rank succ() { return _succ; }
    virtual void setSucc(Rank _succ) { this->_succ = _succ; }
    constexpr static bool isBidirectional() { return false; }
};

// 双向节点
template <typename T>
class StaticListNode : public ForwardStaticListNode<T> {
protected:
    Rank _pred = invalidRank;
public:
    StaticListNode() {}
    StaticListNode(T _data): ForwardStaticListNode<T>(_data) {}
    virtual Rank pred() { return _pred; }
    virtual void setPred(Rank _pred) { this->_pred = _pred; }
    constexpr static bool isBidirectional() { return true; }
};

// 静态链表需要解决的问题是，被删除的节点仍然会在内存中占据空间
// 要重新利用这些空间，就需要引入一个额外的数据结构来存储这些已经被释放的位置

// 静态列表
template <typename T, typename Node = StaticListNode<T>, typename Container = Vector<Node>>
requires (is_base_of_v<AbstractStaticListNode<T>, Node> &&
          clazy_framework::is_listnode_type<T, Node> && equality_comparable<Node> &&
          is_base_of_v<clazy_framework::AbstractVector<Node>, Container>)
class StaticList : public BasicList<T, Rank, Node> {
protected:
    Container V;  // 使用向量存储所有的节点
    Stack<Rank> S; // 使用栈存储被删除的位置

    // 寻找一个可用的位置
    virtual Rank allocatePosition() {
        if (!S.empty()) {    // 如果存在可用的位置
            return S.pop();  // 就弹出一个可用的位置使用
        } else {             // 如果没有可用的位置
            V.resize(V.size() + 1); // 则申请一个新的位置（向量长度增加）
            return V.size() - 1;
        }
    }

public:
    // 重载找位置相关的函数
    virtual T& data(Rank pos) const { return V[pos].data(); }
    virtual Rank pred(Rank pos) const { return V[pos].pred(); }
    virtual Rank succ(Rank pos) const { return V[pos].succ(); }
    virtual Rank setPred(Rank pos, Rank _pred) { 
        if (pos != invalidRank) {
            V[pos].setPred(_pred);
        }
        return pos;
    }
    virtual Rank setSucc(Rank pos, Rank _succ) { 
        if (pos != invalidRank) {
            V[pos].setSucc(_succ);
        }
        return pos;
    }
    virtual Rank invalid() const { return invalidRank; }

    // 重载空间分配相关的函数
    virtual Rank create() { return allocatePosition(); }
    virtual Rank create(const T& e) {
        Rank pos = allocatePosition();
        data(pos) = e;
        return pos;
    }
    virtual void destroy(Rank pos) { S.push(pos); } // 将pos标记为可用
    virtual void destroyAll() { V.clear(); S.clear(); } // 清空所有的元素

    StaticList() { this->createEmptyList(); } // 默认构造函数
    StaticList(const StaticList<T, Node>& L) { this->duplicateList(L); } // 复制构造函数

    template <typename Container1>
    requires (clazy_framework::is_linear_structure<T, Container1>)
    auto& operator=(const Container1& L) {
        this->duplicateList(L); 
        return *this;
    } // 线性表类型转换

};

}
