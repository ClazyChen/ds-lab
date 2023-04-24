module;
#include <functional>

export module BinaryTree.Traverse.BinaryTreePreOrderTraverse;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Traverse.AbstractBinaryTreeTraverse;
import Stack;

export namespace dslab {

template <typename T>
class BinaryTreePreOrderTraverse : public AbstractBinaryTreeTraverse<T> {
public:
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        if (!p) {
            return;
        }
        this->call(visit, p);
        operator()(p->left(), visit);
        operator()(p->right(), visit);
    }
};

template <typename T>
class BinaryTreePreOrderTraverseSemilinear : public AbstractBinaryTreeTraverse<T> {
public:
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S { p };
        while (!S.empty()) {
            if (auto node { S.pop() }; node) {
                this->call(visit, node);
                S.push(node->right());
                S.push(node->left());
            }
        }
    }
};

template <typename T>
class BinaryTreePreOrderTraverseLinear : public AbstractBinaryTreeTraverse<T> {
public:
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S { p };
        while (!S.empty()) {
            for (auto node { S.pop() }; node; node = node->left()) {
                this->call(visit, node);
                S.push(node->right());
            }
        }
    }
};

template <typename T>
class BinaryTreePreOrderTraverseLinearRecursive : public AbstractBinaryTreeTraverse<T> {
public:
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        if (p == nullptr) {
            return;
        }
        while (p->left() != nullptr) {
            this->call(visit, p);
            p = p->left();
        }
        visit(p->data());
        while (p != nullptr) {
            operator()(p->right(), visit);
            p = p->parent();
        }
    }
};

}