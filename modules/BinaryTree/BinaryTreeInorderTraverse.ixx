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
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        if (!p) {
            return;
        }
        operator()(p->left(), visit);
        visit(p);
        operator()(p->right(), visit);
    }
};

template <typename T>
class BinaryTreeInOrderTraverseLinear : public AbstractBinaryTreeTraverse<T> {
    void pushLeftChain(Stack<BinaryTreeNodeConstPos<T>>& S, BinaryTreeNodeConstPos<T> p) {
        while (p) {
            S.push(p);
            p = p->left();
        }
    }
public:
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S;
        pushLeftChain(S, p);
        while (!S.empty()) {
            auto node { S.pop() };
            visit(node);
            pushLeftChain(S, node->right());
        }
    }
};

}