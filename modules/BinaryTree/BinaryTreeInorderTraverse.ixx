export module BinaryTree.Traverse.BinaryTreeInOrderTraverse;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Traverse.AbstractBinaryTreeTraverse;
import Stack;
import std;

export namespace dslab {

template <typename T>
class BinaryTreeInOrderTraverse : public AbstractBinaryTreeTraverse<T> {
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        if (!p) {
            return;
        }
        traverse(p->left(), visit);
        visit(p);
        traverse(p->right(), visit);
    }
    std::string type_name() const override {
        return "InOrder Traverse";
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
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S;
        pushLeftChain(S, p);
        while (!S.empty()) {
            auto node { S.pop() };
            visit(node);
            pushLeftChain(S, node->right());
        }
    }
    std::string type_name() const override {
        return "InOrder Traverse (Iterative)";
    }
};

}