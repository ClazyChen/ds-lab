#pragma once

#include "binary_tree_framework.h"

// 这个文件给出了一个二叉树的实现

namespace clazy {

using clazy_framework::BinaryTreeNodePos;

// 基本的二叉树
template <typename T>
class BinaryTree : public clazy_framework::AbstractBinaryTree<T> {
private:
    using Node = clazy_framework::BinaryTreeNode<T>;
    using BaseTree = clazy_framework::AbstractBinaryTree<T>;
    
    BinaryTreeNodePos<T> _root; // 根节点的位置
    int _size;                  // 树的规模

    // 复制以某个子节点为根的子树，并返回复制的根节点
    BinaryTreeNodePos<T> duplicate(BinaryTreeNodePos<T> pos);

    // 计算以某个子节点为根的子树规模
    int size(BinaryTreeNodePos<T> pos);

    // 将某个节点摘出（不更新规模）
    void detach(BinaryTreeNodePos<T> pos) {
        if (_root == pos) {
            _root = nullptr;
        } else {
            pos->detach();
        }
    }

public:
    BinaryTree() : _root(nullptr), _size(0) {}
    BinaryTree(const T& e) : _root(new Node(e)), _size(1) {}
    BinaryTree(const BinaryTree& BT);
    BinaryTree(BinaryTree&& BT);
    ~BinaryTree() { clear(); }
    BinaryTree& operator=(const BinaryTree& BT);
    BinaryTree& operator=(BinaryTree&& BT);

    int size() const override { return _size; }
    void clear() override { remove(_root); }
    BinaryTreeNodePos<T> root() const override { return _root; }
    void setRoot(BinaryTreeNodePos<T> root) override;
    void updateSize() override { _size = size(_root); }

    void insertAsLeft(BinaryTreeNodePos<T> pos, const T& e) override;
    void insertAsRight(BinaryTreeNodePos<T> pos, const T& e) override;
    void insertAsRoot(const T& e) override;

    void attachAsLeft(BinaryTreeNodePos<T> pos, BaseTree&& tree) override;
    void attachAsRight(BinaryTreeNodePos<T> pos, BaseTree&& tree) override;
    void attachAsRoot(BaseTree&& tree) override;

    int remove(BinaryTreeNodePos<T> pos) override;
    void detach(BinaryTreeNodePos<T> pos, BaseTree& dst) override;

    BinaryTreeNodePos<T> find(const T& e) const override;
};

// 复制以某个子节点为根的二叉树，并返回复制的根节点。采用先序遍历的思路
template <typename T>
BinaryTreeNodePos<T> BinaryTree<T>::duplicate(BinaryTreeNodePos<T> pos) {
    if (pos == nullptr) {
        return nullptr;
    } else {
        auto r = new Node(node->data());
        r->setLeft(duplicate(node->left()));
        r->setRight(duplicate(node->right()));
        return r;
    }
}

// 计算以某个子节点为根的子树规模，采用先序遍历的思路
template <typename T>
int BinaryTree<T>::size(BinaryTreeNodePos<T> pos) {
    if (pos == nullptr) {
        return 0;
    } else {
        return 1 + size(node->left()) + size(node->right());
    }
}

// 复制构造函数、移动构造函数及相应的赋值运算符
template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& BT) {
    _root = duplicate(BT._root);
    _size = BT._size;
}

template <typename T>
BinaryTree<T>::BinaryTree(BinaryTree&& BT) {
    attachAsRoot(BT);
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& BT) {
    if (this != &BT) {
        clear();
        _root = duplicate(BT._root);
        _size = BT._size;
    }
    return *this;
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree&& BT) {
    if (this != &BT) {
        attachAsRoot(BT);
    }
    return *this;
}

// 修改根节点
template <typename T>
void BinaryTree<T>::setRoot(BinaryTreeNodePos<T> root) {
    _root = root;
    updateSize();
}

// 插入一个元素在某个子节点的左子树、右子树或树根的位置上
template <typename T>
void BinaryTree<T>::insertAsLeft(BinaryTreeNodePos<T> pos, const T& e) {
    if (pos == nullptr) {
        insertAsRoot(e);
    } else {
        remove(pos->left());
        pos->setLeft(new Node(e));
        _size++;
    }
}

template <typename T>
void BinaryTree<T>::insertAsRight(BinaryTreeNodePos<T> pos, const T& e) {
    if (pos == nullptr) {
        insertAsRoot(e);
    } else {
        remove(pos->right());
        pos->setRight(new Node(e));
        _size++;
    }
}

template <typename T>
void BinaryTree<T>::insertAsRoot(const T& e) {
    remove(_root);
    _root = new Node(e);
    _size++;
}

// 将一个二叉树插入到某个子节点的左子树、右子树或树根的位置上
template <typename T>
void BinaryTree<T>::attachAsLeft(BinaryTreeNodePos<T> pos, BaseTree&& tree) {
    if (pos == nullptr) {
        attachAsRoot(tree);
    } else {
        remove(pos->left());
        pos->setLeft(tree.root());
        _size += tree.size();
        tree.setRoot(nullptr);
    }
}

template <typename T>
void BinaryTree<T>::attachAsRight(BinaryTreeNodePos<T> pos, BaseTree&& tree) {
    if (pos == nullptr) {
        attachAsRoot(tree);
    } else {
        remove(pos->right());
        pos->setRight(tree.root());
        _size += tree.size();
        tree.setRoot(nullptr);
    }
}

template <typename T>
void BinaryTree<T>::attachAsRoot(BaseTree&& tree) {
    remove(_root);
    _root = tree.root();
    _size += tree.size();
    tree.setRoot(nullptr);
}

// 删除节点及其子树，返回被删除的子树的规模，采用后序遍历的思路
template <typename T>
int BinaryTree<T>::remove(BinaryTreeNodePos<T> pos) {
    if (pos == nullptr) {
        return 0;
    } else {
        int s = 1 + remove(pos->left()) + remove(pos->right());
        detach(pos);
        delete pos;
        _size--;
        return s;
    }
}

// 分离节点到目标子树中
template <typename T>
void BinaryTree<T>::detach(BinaryTreeNodePos<T> pos, BaseTree& tree) {
    if (pos == nullptr) {
        return;
    } else {
        detach(pos);
        tree.clear();
        tree.setRoot(pos);
        _size -= tree.size();
    }
}

// 查找某个元素的位置，采用先序遍历的思路
template <typename T>
BinaryTreeNodePos<T> BinaryTree<T>::find(const T& e) const {
    function<BinaryTreeNodePos<T>(BinaryTreeNodePos<T>)> searcher;
    searcher = [&](auto pos) {
        if (pos == nullptr || pos->data() == e) {
            return pos;
        } else {
            auto p = searcher(pos->left());
            if (p == nullptr) {
                p = searcher(pos->right());
            }
            return p;
        }
    };
    return searcher(_root);
}

}