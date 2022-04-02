#pragma once

#include "tree_framework.h"

namespace clazy_framework {

// 二叉树的抽象类
template <typename T>
class AbstractBinaryTree : public AbstractTree<T, BinaryTreeNode<T>> {
public:
    // 插入为子节点或树根
    virtual void insertAsLC(BinaryTreeNodePos<T> parent, const T& e) = 0;
    virtual void insertAsRC(BinaryTreeNodePos<T> parent, const T& e) = 0;
    virtual void insertAsRoot(const T& e) = 0;

    // 将另一棵树接在某个子节点下，注意这里需要让被接入的树失去控制权，否则会导致重复删除
    // virtual void attachAsLC(TreeNodePos<T> parent, AbstractBinaryTree<T>&& BT) = 0;
    // virtual void attachAsRC(TreeNodePos<T> parent, AbstractBinaryTree<T>&& BT) = 0;

    // 删除某个节点及其为树根的子树，并返回被删除的节点数量
    virtual int remove(BinaryTreeNodePos<T> node) = 0;

    // 将某个节点为根的子树从当前的树上分离
    // virtual void detach(TreeNodePos<T> x, AbstractBinaryTree<T>& dst) = 0;
};

template <typename T, typename Container>
concept is_binary_tree = is_base_of_v<AbstractBinaryTree<T>, Container>;

}