#pragma once

#include "tree_framework.h"

namespace clazy_framework {

// 二叉树的迭代器
template <typename T>
class AbstractBinaryTreeIterator {
protected:
    TreeNodePos<T> _pos;
public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = TreeNodePos<T>;
    using reference         = T&;

    explicit AbstractBinaryTreeIterator(TreeNodePos<T> pos) : _pos(pos) {}

    bool operator==(AbstractBinaryTreeIterator other) {
        return (_pos == other._pos);
    }

    bool operator!=(AbstractBinaryTreeIterator other) {
        return !(*this == other);
    }

    // 您需要自己定义++、+=、--、-=、+、-这些运算符

    reference operator*() {
        return _pos->data();
    }

    pointer operator->() {
        return _pos;
    }

    pointer base() {
        return _pos;
    }
};

// 二叉树的抽象类
template <typename T>
class AbstractBinaryTree : public AbstractTree<T> {
public:
    // 插入为子节点或树根
    virtual void insertAsLC(TreeNodePos<T> parent, const T& e) = 0;
    virtual void insertAsRC(TreeNodePos<T> parent, const T& e) = 0;
    virtual void insertAsRoot(const T& e) = 0;

    // 将另一棵树接在某个子节点下，注意这里需要让被接入的树失去控制权，否则会导致重复删除
    // virtual void attachAsLC(TreeNodePos<T> parent, AbstractBinaryTree<T>&& BT) = 0;
    // virtual void attachAsRC(TreeNodePos<T> parent, AbstractBinaryTree<T>&& BT) = 0;

    // 删除某个节点及其为树根的子树，并返回被删除的节点数量
    virtual int remove(TreeNodePos<T> node) = 0;

    // 将某个节点为根的子树从当前的树上分离
    // virtual void detach(TreeNodePos<T> x, AbstractBinaryTree<T>& dst) = 0;

    // 四种基本的遍历模式，返回一个迭代器，C++23这里应该用views实现
    virtual AbstractBinaryTreeIterator<T> preOrder() const = 0;
    virtual AbstractBinaryTreeIterator<T> inOrder() const = 0;
    virtual AbstractBinaryTreeIterator<T> postOrder() const = 0;
    virtual AbstractBinaryTreeIterator<T> levelOrder() const = 0;

};

}