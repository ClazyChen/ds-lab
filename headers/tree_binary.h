#pragma once

#include "tree_binary_framework.h"

namespace clazy {

template <typename T>
using BinaryTreeNodePos = clazy_framework::BinaryTreeNodePos<T>;

// 二叉树
template <typename T>
class BinaryTree : public clazy_framework::AbstractBinaryTree<T> {
protected:
    BinaryTreeNodePos<T> _root; // 二叉树的树根
    int _size;            // 二叉树的规模
    BinaryTreeNodePos<T> duplicate(BinaryTreeNodePos<T> node) const; // 复制以某个子节点为根的二叉树

public:
    // 获取树的规模
    virtual int size() const override { return _size; }
    
    // 创建节点
    virtual BinaryTreeNodePos<T> create() override { return new clazy_framework::BinaryTreeNode<T>(); }
    virtual BinaryTreeNodePos<T> create(const T& e) override { return new clazy_framework::BinaryTreeNode<T>(e); }
    
    // 获取和设置二叉树的根节点
    virtual BinaryTreeNodePos<T> root() const override { return _root; }
    virtual void setRoot(BinaryTreeNodePos<T> root) override { remove(_root); _root = root; }

    // 插入为子节点或树根（替换原节点）
    virtual void insertAsLC(BinaryTreeNodePos<T> parent, const T& e) override;
    virtual void insertAsRC(BinaryTreeNodePos<T> parent, const T& e) override;
    virtual void insertAsRoot(const T& e) override;

    // 删除节点及其为根的子树，返回被删除的子树的规模
    virtual int remove(BinaryTreeNodePos<T> node) override;

    // 构造函数和赋值函数
    BinaryTree() : _root(nullptr), _size(0) {}
    BinaryTree(const T& e) : _root(new TreeNode<T>(e)), _size(1) {}
    BinaryTree(const BinaryTree<T>& BT) : _root(duplicate(BT._root)), _size(BT._size) {}
    BinaryTree(BinaryTree<T>&& BT) : _root(BT._root), _size(BT._size) {
        BT._root = nullptr;
        BT._size = 0;
    }
    BinaryTree<T>& operator=(const BinaryTree<T>& BT) {
        if (this != &BT) {
            remove(_root);
            _root = duplicate(BT._root);
            _size = BT._size;
        }
        return *this;
    }
    BinaryTree<T>& operator=(BinaryTree<T>&& BT) {
        if (this != &BT) {
            remove(_root);
            _root = BT._root;
            _size = BT._size;
            BT._root = nullptr;
            BT._size = 0;
        }
        return *this;
    }
};

// 复制以某个子节点为根的二叉树，返回复制的树的根，使用先序遍历实现
template <typename T>
BinaryTreeNodePos<T> BinaryTree<T>::duplicate(BinaryTreeNodePos<T> node) const {
    if (node == nullptr) {
        return nullptr;
    }
    auto newNode = create(node->getData());
    newNode->setLC(duplicate(node->getLC()));
    newNode->setRC(duplicate(node->getRC()));
    return newNode;
}

template <typename T>
void BinaryTree<T>::insertAsLC(BinaryTreeNodePos<T> parent, const T& e) {
    auto node = create(e);
    if (parent == nullptr) {
        setRoot(node);
    } else {
        if (parent->getLC() != nullptr) {
            remove(parent->getLC());
        }
        parent->setLC(node);
    }
    _size++;
}

// 删除节点及其为根的子树，返回被删除的子树的规模，使用后序遍历实现
template <typename T>
int BinaryTree<T>::remove(BinaryTreeNodePos<T> node) {
    if (node == nullptr) {
        return 0;
    }
    int n = 1;
    n += remove(node->getLC());
    n += remove(node->getRC());
    delete node;
    _size -= n;
    return n;
}

// 利用<<输出以某个节点为根的子树，采用先序遍历进行序列化
template <typename T>
ostream& operator<<(ostream& out, const BinaryTreeNodePos<T>& node) {
    if (node == nullptr) {
        out << "null";
    } else if (node->getLC() == nullptr && node->getRC() == nullptr) {
        out << node->getData();
    } else {
        out << "(" << node->getData() << ", ";
        out << node->getLC() << ", ";
        out << node->getRC() << ")";
    }
    return out;
}

// 利用<<输出整棵树
template <typename T>
ostream& operator<<(ostream& out, const BinaryTree<T>& BT) {
    out << BT.root();
    return out;
}


}