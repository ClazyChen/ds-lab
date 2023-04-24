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
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        if (!p) {
            return;
        }
        visit(p);
        traverse(p->left(), visit);
        traverse(p->right(), visit);
    }
};

template <typename T>
class BinaryTreePreOrderTraverseSemilinear : public AbstractBinaryTreeTraverse<T> {
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S { p };
        while (!S.empty()) {
            if (auto node { S.pop() }; node) {
                visit(node);
                S.push(node->right());
                S.push(node->left());
            }
        }
    }
};

template <typename T>
class BinaryTreePreOrderTraverseLinear : public AbstractBinaryTreeTraverse<T> {
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S { p };
        while (!S.empty()) {
            for (auto node { S.pop() }; node; node = node->left()) {
                visit(node);
                S.push(node->right());
            }
        }
    }
};

template <typename T>
class BinaryTreePreOrderTraverseLinearRecursive : public AbstractBinaryTreeTraverse<T> {
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        if (p == nullptr) {
            return;
        }
        while (p->left() != nullptr) {
            visit(p);
            p = p->left();
        }
        visit(p);
        while (p != nullptr) {
            traverse(p->right(), visit);
            p = p->parent();
        }
    }
};

}