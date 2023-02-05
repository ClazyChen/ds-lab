module;
#include <memory>
#include <functional>

export module BinaryTree.BinaryTreeInorderTraverse;

import BinaryTree.BinaryTreeNode;
import BinaryTree.AbstractBinaryTree;
import BinaryTree.BinaryTreeTraverse;
import Stack;

export namespace dslab {

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreeInorderTraverse : public BinaryTreeTraverse<T, Node> {
    void operator()(const Node<T>* node, std::function<void(const T&)> visit) override {
        if (node == nullptr) {
            return;
        }
        (*this)(node->left().get(), visit);
        visit(node->data());
        (*this)(node->right().get(), visit);
    }
};

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreeInorderTraverseLinear : public BinaryTreeTraverse<T, Node> {
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
            visit(t->data());
            pushLeftChain(S, t->right().get());
        }
    }
};

}