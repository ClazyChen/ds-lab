module;
#include <memory>
#include <functional>

export module BinaryTree.BinaryTreeLevelOrderTraverse;

import BinaryTree.AbstractBinaryTreeNode;
import BinaryTree.AbstractBinaryTree;
import BinaryTree.BinaryTreeTraverse;
import Queue;

export namespace dslab {

template <typename T>
class BinaryTreeLevelOrderTraverse : public BinaryTreeTraverse<T> {
    void operator()(const AbstractBinaryTreeNode<T>* node, std::function<void(const T&)> visit) override {
        Queue<AbstractBinaryTreeNode<T>*> Q { node };
        while (!Q.empty()) {
            if (auto t { Q.dequeue() }; t != nullptr) {
                visit(t->data());
                Q.enqueue(t->left().get());
                Q.enqueue(t->right().get());
            }
        }
    }
};

}