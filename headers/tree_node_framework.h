#pragma once

#include "vector.h"

namespace clazy_framework {

// 在二叉树和树这里，采用的定义方式是树->K叉树->二叉树的三级结构
// 这里就不讨论用静态方法实现的树了，那种方法和静态链表相似
// 在树的定义里增加一个模板参数P，然后换一个抽象节点类就可以了
// 我认为，既然您已经理解了列表部分的Node设计，就没有必要再重复去做树部分的Node设计
// 所以我把树的Node（在我的设计中总是包含parent指针）封装到了框架里

// 首先定义一个统一的抽象的树节点类，然后有K叉树的节点和动态树的节点分别继承于这个类
// 二叉树节点继承于K叉树节点；而动态树节点则用来构成一般的树的结构
template <typename T>
class AbstractTreeNode {
protected:
    T _data;
public:
    // 用指针表示树上的位置
    using Pos = AbstractTreeNode<T>*;

    // 获取数据的接口
    virtual T& data() const { return _data; }

    // 子节点的数量
    virtual int childCount() const = 0;

    // 获取和设置子节点、亲节点的接口
    virtual Pos child(int index) const = 0;
    virtual Pos parent() const = 0;
    virtual void setChild(Pos child, int index) = 0;
    virtual void setParent(Pos parent) = 0;

    // 插入和删除子节点，用于动态树
    virtual void insertChild(Pos child, int index) { setChild(child, index); }
    virtual void removeChild(int index) { setChild(nullptr, index); }

    // 判断是否是树根（没有parent）
    virtual bool isRoot() const { return parent() == nullptr; }
    virtual bool hasParent() const { return parent() != nullptr; }

    // 构造函数
    AbstractTreeNode() {}
    AbstractTreeNode(const T& data) : _data(data) {}

    // 比较接口
    bool operator==(const AbstractTreeNode<T>& other) const { return _data == other._data; }
    auto operator<=>(const AbstractTreeNode<T>& other) const = default;
};

// 用指针表示树上的位置
template <typename T>
using TreeNodePos = AbstractTreeNode<T>::Pos;

// K叉节点
template <typename T, int K>
requires (K > 1)
class KTreeNode : public AbstractTreeNode<T> {
protected:
    AbstractTreeNode<T>* _children[K] { nullptr };
    AbstractTreeNode<T>* _parent = nullptr;
public:
    virtual int childCount() const override { // K个子节点中非nullptr的数量
        return count_if(_children, _children + K, [](auto child) { return child != nullptr; });
    }
    virtual AbstractTreeNode<T>* child(int index) const override { return _children[index]; }
    virtual AbstractTreeNode<T>* parent() const override { return _parent; }
    virtual void setChild(AbstractTreeNode<T>* child, int index) override { _children[index] = child; }
    virtual void setParent(AbstractTreeNode<T>* parent) override { _parent = parent; }

    // 构造函数
    KTreeNode() {}
    KTreeNode(const T& data) : AbstractTreeNode<T>(data) {}
};

// 二叉节点，作为特殊的K叉节点而存在
template <typename T>
class BinaryTreeNode : public KTreeNode<T, 2> {
private:
    // 把原先的child和setChild隐藏为私有方法
    virtual AbstractTreeNode<T>* child(int index) const override { return KTreeNode<T, 2>::child(index); }
    virtual void setChild(AbstractTreeNode<T>* child, int index) override { KTreeNode<T, 2>::setChild(child, index); }
public:
    // 获取和设置子节点、亲节点的接口
    virtual AbstractTreeNode<T>* LC() const { return _children[0]; }
    virtual AbstractTreeNode<T>* RC() const { return _children[1]; }
    virtual bool hasLC() const { return LC() != nullptr; }
    virtual bool hasRC() const { return RC() != nullptr; }
    virtual void setLC(AbstractTreeNode<T>* child) { setChild(child, 0); }
    virtual void setRC(AbstractTreeNode<T>* child) { setChild(child, 1); }

    // 判断是左子节点还是右子节点的接口
    virtual bool isLC() const { return parent() != nullptr && parent()->LC() == this; }
    virtual bool isRC() const { return parent() != nullptr && parent()->RC() == this; }

    // 构造函数
    BinaryTreeNode() {}
    BinaryTreeNode(const T& data) : KTreeNode<T, 2>(data) {}
};

// 动态节点
template <typename T, typename Container = clazy::Vector<AbstractTreeNode<T>*>>
requires (is_base_of_v<AbstractVector<AbstractTreeNode<T>*>, Container>)
class DynamicTreeNode : public AbstractTreeNode<T> {
protected:
    Container _children;
    AbstractTreeNode<T>* _parent = nullptr;
public:
    virtual int childCount() const override { return _children.size(); }
    virtual AbstractTreeNode<T>* child(int index) const override { return _children[index]; }
    virtual AbstractTreeNode<T>* parent() const override { return _parent; }
    virtual void setChild(AbstractTreeNode<T>* child, int index) override { _children[index] = child; }
    virtual void setParent(AbstractTreeNode<T>* parent) override { _parent = parent; }

    // 构造函数
    DynamicTreeNode() {}
    DynamicTreeNode(const T& data) : AbstractTreeNode<T>(data) {}

    // 插入一个子节点（移动它的后缀）
    virtual void insertChild(AbstractTreeNode<T>* child, int index) override { _children.insert(index, child); }
    // 删除一个子节点（移动它的后缀）
    virtual void removeChild(int index) override { _children.remove(index); }
};

}