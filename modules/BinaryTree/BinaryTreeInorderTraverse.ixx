module;
#include <memory>
#include <functional>

export module BinaryTree.BinaryTreeInorderTraverse;

import BinaryTree.AbstractBinaryTreeNode;
import BinaryTree.AbstractBinaryTree;
import BinaryTree.BinaryTreeTraverse;
import Stack;

export namespace dslab {

template <typename T>
class BinaryTreeInorderTraverse : public BinaryTreeTraverse<T> {
    void operator()(const AbstractBinaryTreeNode<T>* node, std::function<void(const T&)> visit) override {
        if (node == nullptr) {
            return;
        }
        (*this)(node->left().get(), visit);
        visit(node->data());
        (*this)(node->right().get(), visit);
    }
};

template <typename T>
class BinaryTreeInorderTraverseLinear : public BinaryTreeTraverse<T> {
    void pushLeftChain(Stack<AbstractBinaryTreeNode<T>*>& S, const AbstractBinaryTreeNode<T>* node) {
        for (auto t { node }; t != nullptr; t = t->left().get()) {
            S.push(t);
        }
    }
public:
    void operator()(const AbstractBinaryTreeNode<T>* node, std::function<void(const T&)> visit) override {
        Stack<AbstractBinaryTreeNode<T>*> S;
        pushLeftChain(S, node);
        while (!S.empty()) {
            auto t { S.pop() };
            visit(t->data());
            pushLeftChain(S, t->right().get());
        }
    }
};

}