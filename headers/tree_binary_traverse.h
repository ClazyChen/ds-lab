#pragma once

#include "tree_binary.h"
#include "queue.h"

namespace clazy_framework {

// 进行二叉树的遍历的抽象类
template <typename T, typename Container>
requires (is_binary_tree<T, Container>)
class BinaryTreeTraverse : public Algorithm {
protected:
    // 二叉树遍历使用的函数
    function<void(BinaryTreeNodePos<T>)> visit;
    // 用于重载的函数，遍历以node为根的子树
    virtual void traverse(BinaryTreeNodePos<T> node) const = 0;
public:
    BinaryTreeTraverse(function<void(BinaryTreeNodePos<T>)> visit) : visit(visit) {}
    virtual void apply(Container& tree) const {
        if (!tree.empty()) {
            traverse(tree.root());
        }
    }
};

}

namespace clazy {

// 经典先序遍历（递归）
template <typename T, typename Container = BinaryTree<T>>
class PreOrderTraverse : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    virtual void traverse(BinaryTreeNodePos<T> node) const override {
        if (node != nullptr) {
            this->visit(node);
            this->traverse(node->getLC());
            this->traverse(node->getRC());
        }
    }
};

// 经典中序遍历（递归）
template <typename T, typename Container = BinaryTree<T>>
class InOrderTraverse : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    virtual void traverse(BinaryTreeNodePos<T> node) const override {
        if (node != nullptr) {
            this->traverse(node->getLC());
            this->visit(node);
            this->traverse(node->getRC());
        }
    }
};

// 经典后序遍历（递归）
template <typename T, typename Container = BinaryTree<T>>
class PostOrderTraverse : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    virtual void traverse(BinaryTreeNodePos<T> node) const override {
        if (node != nullptr) {
            this->traverse(node->getLC());
            this->traverse(node->getRC());
            this->visit(node);
        }
    }
};

// 经典层次遍历（利用队列）
template <typename T, typename Container = BinaryTree<T>>
class LevelOrderTraverse : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    virtual void traverse(BinaryTreeNodePos<T> node) const override {
        if (node != nullptr) {
            Queue<BinaryTreeNodePos<T>> Q;
            Q.enqueue(node);
            while (!Q.empty()) {
                auto cur = Q.dequeue();
                this->visit(cur);
                if (cur->getLC() != nullptr) {
                    Q.enqueue(cur->getLC());
                }
                if (cur->getRC() != nullptr) {
                    Q.enqueue(cur->getRC());
                }
            }
        }
    }
};


}
