module;
#include <memory>
#include <functional>

export module BinaryTree.BinaryTreePostorderTraverse;

import BinaryTree.BinaryTreeNode;
import BinaryTree.AbstractBinaryTree;
import BinaryTree.BinaryTreeTraverse;
import Stack;

export namespace dslab {

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreePostorderTraverse : public BinaryTreeTraverse<T, Node> {
    void operator()(const Node<T>* node, std::function<void(const T&)> visit) override {
        if (node == nullptr) {
            return;
        }
        (*this)(node->left().get(), visit);
        (*this)(node->right().get(), visit);
        visit(node->data());
    }
};

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreePostorderTraverseSemilinear : public BinaryTreeTraverse<T, Node> {
    void operator()(const Node<T>* node, std::function<void(const T&)> visit) override {
        Stack<Node<T>*> S;
        Stack<Node<T>*> St;
        S.push(node);
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

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreePostorderTraverseLinear : public BinaryTreeTraverse<T, Node> {
    void pushLeftChain(Stack<Node<T>*>& S, const Node<T>* node) {
        for (auto t { node }; t != nullptr; t = t->left().get()) {
            S.push(t);
        }
    }
public:
    void operator()(const Node<T>* node, std::function<void(const T&)> visit) override {
        Stack<Node<T>*> S;
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