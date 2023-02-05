module;
#include <memory>
#include <functional>

export module BinaryTree.BinaryTreePreorderTraverse;

import BinaryTree.BinaryTreeNode;
import BinaryTree.AbstractBinaryTree;
import BinaryTree.BinaryTreeTraverse;
import Stack;

export namespace dslab {

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreePreorderTraverse : public BinaryTreeTraverse<T, Node> {
    void operator()(const Node<T>* node, std::function<void(const T&)> visit) override {
        if (node == nullptr) {
            return;
        }
        visit(node->data());
        (*this)(node->left().get(), visit);
        (*this)(node->right().get(), visit);
    }
};

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreePreorderTraverseSemilinear : public BinaryTreeTraverse<T, Node> {
    void operator()(const Node<T>* node, std::function<void(const T&)> visit) override {
        Stack<Node<T>*> S;
        S.push(node);
        while (!S.empty()) {
            if (auto t { S.pop() }; t != nullptr) {
                visit(t->data());
                S.push(t->right().get());
                S.push(t->left().get());
            }
        }
    }
};

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreePreorderTraverseLinear : public BinaryTreeTraverse<T, Node> {
    void operator()(const Node<T>* node, std::function<void(const T&)> visit) override {
        Stack<Node<T>*> S;
        S.push(node);
        while (!S.empty()) {
            for (auto t { S.pop() }; t != nullptr; t = t->left().get()) {
                visit(t->data());
                S.push(t->right().get());
            }
        }
    }
};

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreePreorderTraverseLinearRecursive : public BinaryTreeTraverse<T, Node> {
    void operator()(const Node<T>* node, std::function<void(const T&)> visit) override {
        if (node == nullptr) {
            return;
        }
        while (node->left() != nullptr) {
            visit(node->data());
            node = node->left().get();
        }
        visit(node->data());
        while (node != nullptr) {
            (*this)(node->right().get(), visit);
            node = node->parent();
        }
    }
};

}