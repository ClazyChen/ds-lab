module;
#include <memory>
#include <functional>

export module BinaryTree.BinaryTreePostorderTraverse;

import BinaryTree.AbstractBinaryTreeNode;
import BinaryTree.AbstractBinaryTree;
import BinaryTree.BinaryTreeTraverse;
import Stack;

export namespace dslab {

template <typename T>
class BinaryTreePostorderTraverse : public BinaryTreeTraverse<T> {
    void operator()(const AbstractBinaryTreeNode<T>* node, std::function<void(const T&)> visit) override {
        if (node == nullptr) {
            return;
        }
        (*this)(node->left().get(), visit);
        (*this)(node->right().get(), visit);
        visit(node->data());
    }
};

template <typename T>
class BinaryTreePostorderTraverseSemilinear : public BinaryTreeTraverse<T> {
    void operator()(const AbstractBinaryTreeNode<T>* node, std::function<void(const T&)> visit) override {
        Stack<const AbstractBinaryTreeNode<T>*> S { node };
        Stack<const AbstractBinaryTreeNode<T>*> St;
        while (!S.empty()) {
            if (auto t { S.pop() }; t != nullptr) {
                St.push(t);
                S.push(t->left().get());
                S.push(t->right().get());
            }
        }
        while (!St.empty()) {
            visit(St.pop()->data());
        }
    }
};

template <typename T>
class BinaryTreePostorderTraverseLinear : public BinaryTreeTraverse<T> {
    void pushLeftChain(Stack<const AbstractBinaryTreeNode<T>*>& S, const AbstractBinaryTreeNode<T>* node) {
        for (auto t { node }; t != nullptr; t = t->left().get()) {
            S.push(t);
        }
    }
public:
    void operator()(const AbstractBinaryTreeNode<T>* node, std::function<void(const T&)> visit) override {
        Stack<const AbstractBinaryTreeNode<T>*> S;
        pushLeftChain(S, node);
        while (!S.empty()) {
            auto t { S.pop() };
            if (t->right() == nullptr) {
                visit(t->data());
                while (!S.empty() && S.top()->right().get() == t) {
                    t = S.pop();
                    visit(t->data());
                }
            } else {
                S.push(t);
                pushLeftChain(S, t->right().get());
            }
        }
    }
};

}