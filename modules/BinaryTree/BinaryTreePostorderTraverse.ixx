export module BinaryTree.Traverse.BinaryTreePostOrderTraverse;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Traverse.AbstractBinaryTreeTraverse;
import Stack;
import std;

export namespace dslab {

template <typename T>
class BinaryTreePostOrderTraverse : public AbstractBinaryTreeTraverse<T> {
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        if (!p) {
            return;
        }
        traverse(p->left(), visit);
        traverse(p->right(), visit);
        visit(p);
    }
    std::string type_name() const override {
        return "PostOrder Traverse";
    }
};

template <typename T>
class BinaryTreePostOrderTraverseSemilinear : public AbstractBinaryTreeTraverse<T> {
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
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
    std::string type_name() const override {
        return "PostOrder Traverse (Iterative / Semilinear)";
    }
};

template <typename T>
class BinaryTreePostOrderTraverseLinear : public AbstractBinaryTreeTraverse<T> {
    void pushLeftChain(Stack<BinaryTreeNodeConstPos<T>>& S, BinaryTreeNodeConstPos<T> p) {
        while (p) {
            S.push(p);
            p = p->left();
        }
    }
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        Stack<BinaryTreeNodeConstPos<T>> S;
        BinaryTreeNodeConstPos<T> last { nullptr };
        pushLeftChain(S, p);
        while (!S.empty()) {
            if (auto node { S.top() }; node->left() == last && node->right()) {
                pushLeftChain(S, node->right());
            } else {
                last = S.pop();
                visit(last);
            }
        }
    }
    std::string type_name() const override {
        return "PostOrder Traverse (Iterative / Linear)";
    }
};

}