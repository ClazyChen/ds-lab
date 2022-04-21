#pragma once

#include "binary_tree_iterator_framework.h"

// 这个文件给出了二叉树的基本框架
// 它需要实现二叉树的下列基本操作
// 1. 获取或设置树根的位置
// 2. 将新元素插入为某个节点的左子或右子或树根（替换原子树）
// 3. 将一棵树接入为某个节点的左子或右子或树根（替换原子树）
// 4. 删除某个节点及其为树根的子树，并返回被删除的节点数量
// 5. 将某个节点为根的子树从当前的树上分离，返回被分离的子树
// 6. 查找某个元素（查找成功返回节点位置，否则返回nullptr）

// 这里没有定义二叉树的迭代器
// 因为只有线索二叉树或者亲代+子代的二叉树才能够按照中序次序迭代

namespace clazy_framework {

// 二叉树的ADT接口
template <typename T>
class AbstractBinaryTree : public DataStructure<T> {
public:
    virtual int size() const = 0;
    virtual void clear() = 0;
    virtual BinaryTreeNodePos<T> root() const = 0;
    virtual void setRoot(BinaryTreeNodePos<T> root) = 0;
    virtual void updateSize() = 0; 

    virtual void insertAsLeft(BinaryTreeNodePos<T> pos, const T& e) = 0;
    virtual void insertAsRight(BinaryTreeNodePos<T> pos, const T& e) = 0;
    virtual void insertAsRoot(const T& e) = 0;

    virtual void attachAsLeft(BinaryTreeNodePos<T> pos, AbstractBinaryTree<T>&& tree) = 0;
    virtual void attachAsRight(BinaryTreeNodePos<T> pos, AbstractBinaryTree<T>&& tree) = 0;
    virtual void attachAsRoot(AbstractBinaryTree<T>&& tree) = 0;

    virtual int remove(BinaryTreeNodePos<T> pos) = 0;
    virtual void detach(BinaryTreeNodePos<T> pos, AbstractBinaryTree<T>& dst) = 0;

    virtual BinaryTreeNodePos<T> find(const T& e) const = 0;

    // 比较接口，使用根节点的值进行比较
    bool operator==(const AbstractBinaryTree<T>& t) const {
        return root() != nullptr && t.root() != nullptr && root()->data() == t.root()->data();
    }

    bool operator!=(const AbstractBinaryTree<T>& t) const {
        return !(*this == t);
    }

    // 迭代器接口
    template <typename IA>
    virtual BinaryTreeIterator<T, IA> begin() const {
        return BinaryTreeIterator<T, IA>(IA().first(root()));
    }

    template <typename IA>
    virtual BinaryTreeIterator<T, IA> end() const {
        return BinaryTreeIterator<T, IA>(nullptr);
    }

    template <typename IA>
    virtual BinaryTreeIterator<T, IA> rbegin() const {
        return BinaryTreeIterator<T, IA>(IA().last(root()));
    }

    template <typename IA>
    virtual BinaryTreeIterator<T, IA> rend() const {
        return BinaryTreeIterator<T, IA>(nullptr);
    }
};

}