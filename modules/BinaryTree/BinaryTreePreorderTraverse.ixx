export module BinaryTree.Traverse.BinaryTreePreOrderTraverse;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Traverse.AbstractBinaryTreeTraverse;
import Stack;
import std;

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
    std::string type_name() const override {
        return "PreOrder Traverse";
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
    std::string type_name() const override {
        return "PreOrder Traverse (Iterative / Semilinear)";
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
    std::string type_name() const override {
        return "PreOrder Traverse (Iterative / Linear)";
    }
};

template <typename T>
class BinaryTreePreOrderTraverseLinearRight : public AbstractBinaryTreeTraverse<T> {
    void pushRightChain(Stack<BinaryTreeNodeConstPos<T>>& S, BinaryTreeNodeConstPos<T> p) {
        Stack<BinaryTreeNodeConstPos<T>> St;
        while (p) {
            St.push(p);
            p = p->right();
        }
        while (!St.empty()) {
            S.push(St.pop());
        }
    }
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S;
        pushRightChain(S, p);
        while (!S.empty()) {
            auto node { S.pop() };
            visit(node);
            pushRightChain(S, node->left());
        }
    }
    std::string type_name() const override {
        return "PreOrder Traverse (Iterative / Linear / Right Chain)";
    }
};

template <typename T>
class BinaryTreePreOrderTraverseLinearRecursive : public AbstractBinaryTreeTraverse<T> {
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        while (p != nullptr) {
            visit(p);
            traverse(p->left(), visit);
            p = p->right();
        }
    }
    std::string type_name() const override {
        return "PreOrder Traverse (Recursive / Linear)";
    }
};

}