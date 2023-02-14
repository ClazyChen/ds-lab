module;
#include <memory>
#include <iterator>

export module BinaryTree.BinaryTreeInorderIterator;

import BinaryTree.AbstractBinaryTreeNode;
import BinaryTree.BinaryTreeIterator;

export namespace dslab {

template <typename T>
class BinaryTreeInorderIterate : public BinaryTreeIterate<T> {
    const AbstractBinaryTreeNode<T>* findLeft(const AbstractBinaryTreeNode<T>* node) {
        while (node->left() != nullptr) {
            node = node->left().get();
        }
        return node;
    }
public:
    const AbstractBinaryTreeNode<T>* begin(const AbstractBinaryTreeNode<T>* node) override {
        return findLeft(node);
    }
    const AbstractBinaryTreeNode<T>* next(const AbstractBinaryTreeNode<T>* node) override {
        if (node == nullptr) {
            return nullptr;
        }
        if (auto right { node->right().get() }; right != nullptr) {
            return findLeft(right);
        } else {
            while (node->isRightChild()) {
                node = node->parent();
            }
            return node->parent();
        }
    }
};

}