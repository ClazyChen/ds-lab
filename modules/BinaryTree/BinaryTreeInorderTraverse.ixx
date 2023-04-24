module;
#include <functional>

export module BinaryTree.Traverse.BinaryTreeInOrderTraverse;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Traverse.AbstractBinaryTreeTraverse;
import Stack;

export namespace dslab {

template <typename T>
class BinaryTreeInOrderTraverse : public AbstractBinaryTreeTraverse<T> {
public:
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        if (!p) {
            return;
        }
        operator()(p->left(), visit);
        this->call(visit, p);
        operator()(p->right(), visit);
    }
};

template <typename T>
class BinaryTreeInOrderTraverseLinear : public AbstractBinaryTreeTraverse<T> {
    void pushLeftChain(Stack<BinaryTreeNodeConstPos<T>>& S, BinaryTreeNodeConstPos<T> p) {
        for (auto node { p }; node; node = node->left()) {
            S.push(node);
        }
    }
public:
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S;
        pushLeftChain(S, p);
        while (!S.empty()) {
            auto node { S.pop() };
            this->call(visit, node);
            pushLeftChain(S, node->right());
        }
    }
};

}