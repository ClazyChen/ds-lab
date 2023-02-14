module;
#include <memory>
#include <iterator>

export module BinaryTree.BinaryTreePostorderIterator;

import BinaryTree.AbstractBinaryTreeNode;
import BinaryTree.BinaryTreeIterator;

export namespace dslab {

template <typename T>
class BinaryTreePostorderIterate : public BinaryTreeIterate<T> {
    AbstractBinaryTreeNode<T>* findLeftMost(AbstractBinaryTreeNode<T>* node) {
        while (!node->isLeaf()) {
            if (node->left() != nullptr) {
                node = node->left().get();
            } else {
                node = node->right().get();
            }
        }
        return node;
    }
public:
    AbstractBinaryTreeNode<T>* begin(AbstractBinaryTreeNode<T>* node) override {
        return findLeftMost(node);
    }
    AbstractBinaryTreeNode<T>* next(AbstractBinaryTreeNode<T>* node) override {
        if (node == nullptr) {
            return nullptr;
        }
        if (auto sib { node->rightSibling() }; sib != nullptr) {
            return findLeftMost(sib);
        } else {
            return node->parent();
        }
    }
};

}