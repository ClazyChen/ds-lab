#pragma once

#include "tree_binary_framework.h"

namespace clazy {

template <typename T>
using TreeNodePos = clazy_framework::TreeNodePos<T>;

// 二叉树
template <typename T>
class BinaryTree : public clazy_framework::AbstractBinaryTree<T> {
protected:
    TreeNodePos<T> _root; // 二叉树的树根
    int _size;            // 二叉树的规模
public:
    // 获取树的规模
    virtual int size() const override { return _size; }
    
    // 创建节点
    virtual TreeNodePos<T> create() override { return new clazy_framework::BinaryTreeNode<T>(); }
    virtual TreeNodePos<T> create(const T& e) override { return new clazy_framework::BinaryTreeNode<T>(e); }
    
    // 获取和设置二叉树的根节点
    virtual TreeNodePos<T> root() const override { return _root; }
    virtual void setRoot(TreeNodePos<T> root) override { _root = root; }

    // 插入为子节点或树根
    virtual void insertAsLC(TreeNodePos<T> parent, const T& e) override {
        TreeNodePos<T> node = create(e);
        if (parent == nullptr) {
            setRoot(node);
        } else {
            parent->setLC(node);
            node->setParent(parent);
        }
        _size++;
    }
    virtual void insertAsRC(TreeNodePos<T> parent, const T& e) override {
        TreeNodePos<T> node = create(e);
        if (parent == nullptr) {
            setRoot(node);
        } else {
            parent->setRC(node);
            node->setParent(parent);
        }
        _size++;
    }
    virtual void insertAsRoot(const T& e) override {
        TreeNodePos<T> node = create(e);
        setRoot(node);
        _size++;
    }

    // 将另一棵树接在某个子节点下
    // virtual void attachAsLC(TreeNodePos<T> parent, AbstractBinaryTree<T>&& BT) override {
    //     if (parent == nullptr) {
    //         setRoot(BT.root());
    //     } else {
    //         parent->setLC(BT.root());
    //         BT.root()->setParent(parent);
    //         BT.setRoot(nullptr);
    //     }
    //     _size += BT.size();
    // }
    // virtual void attachAsRC(TreeNodePos<T> parent, AbstractBinaryTree<T>&& BT) override {
    //     if (parent == nullptr) {
    //         setRoot(BT.root());
    //     } else {
    //         parent->setRC(BT.root());
    //         BT.root()->setParent(parent);
    //         BT.setRoot(nullptr);
    //     }
    //     _size += BT.size();
    // }

    // 删除节点及其为根的子树，返回被删除的子树的规模
    // 这里相当于进行了后序遍历
    virtual int remove(TreeNodePos<T> node) override {
        if (node == nullptr) { // 递归边界
            return 0;
        }
        int size = 1;
        size += remove(node->getLC()); // 后序遍历，LRN
        size += remove(node->getRC());
        delete node;
        _size -= size;
        return size;
    }

    // 构造函数和赋值函数
    BinaryTree() : _root(nullptr), _size(0) {}
    BinaryTree(const T& e) : _root(new TreeNode<T>(e)), _size(1) {}
    BinaryTree(const BinaryTree<T>& BT) : _root(BT._root), _size(BT._size) {}
    BinaryTree(BinaryTree<T>&& BT) : _root(BT._root), _size(BT._size) {
        BT._root = nullptr;
        BT._size = 0;
    }
    BinaryTree<T>& operator=(BinaryTree<T>&& BT) {
        if (this != &BT) {
            _root = BT._root;
            _size = BT._size;
            BT._root = nullptr;
            BT._size = 0;
        }
        return *this;
    }
};

}