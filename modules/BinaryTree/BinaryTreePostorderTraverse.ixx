﻿module;
#include <memory>
#include <functional>

export module BinaryTree.Traverse.BinaryTreePostOrderTraverse;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Traverse.AbstractBinaryTreeTraverse;
import Stack;

export namespace dslab {

template <typename T>
class BinaryTreePostorderTraverse : public AbstractBinaryTreeTraverse<T> {
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        if (!p) {
            return;
        }
        traverse(p->left(), visit);
        traverse(p->right(), visit);
        this->call(visit, p);
    }
};

template <typename T>
class BinaryTreePostorderTraverseSemilinear : public AbstractBinaryTreeTraverse<T> {
public:
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S { p };
        Stack<BinaryTreeNodeConstPos<T>> St;
        while (!S.empty()) {
            if (auto node { S.pop() }; node) {
                St.push(node);
                S.push(node->left());
                S.push(node->right());
            }
        }
        while (!St.empty()) {
            visit(St.pop());
        }
    }
};

template <typename T>
class BinaryTreePostorderTraverseLinear : public AbstractBinaryTreeTraverse<T> {
    void pushLeftChain(Stack<BinaryTreeNodeConstPos<T>>& S, BinaryTreeNodeConstPos<T> p) {
        while (p) {
            S.push(p);
            p = p->left();
        }
    }
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S;
        pushLeftChain(S, p);
        while (!S.empty()) {
            if (auto node { S.pop() }; node->right()) {
                S.push(node);
                pushLeftChain(S, node->right());
            } else {
                visit(node);
                while (!S.empty() && S.top()->right() == node) {
                    node = S.pop();
                    visit(node);
                }
            }
        }
    }
};

}