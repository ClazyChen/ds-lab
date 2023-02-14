module;
#include <memory>
#include <iterator>

export module BinaryTree.BinaryTreePreorderIterator;

import BinaryTree.AbstractBinaryTreeNode;
import BinaryTree.BinaryTreeIterator;

export namespace dslab {

template <typename T>
class BinaryTreePreorderIterate : public BinaryTreeIterate<T> {
public:
    AbstractBinaryTreeNode<T>* begin(AbstractBinaryTreeNode<T>* node) override {
        return node;
    }
    AbstractBinaryTreeNode<T>* next(AbstractBinaryTreeNode<T>* node) override {
        if (node == nullptr) {
            return nullptr;
        }
        if (auto left { node->left().get() }; left != nullptr) {
            return left;
        }
        if (auto right { node->right().get() }; right != nullptr) {
            return right;
        }
        while (node != nullptr) {
            if (auto sib { node->rightSibling() }; sib != nullptr) {
                return sib;
            }
            node = node->parent();
        }
        return nullptr;
    }
};

}