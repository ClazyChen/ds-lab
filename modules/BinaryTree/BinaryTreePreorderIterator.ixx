module;
#include <memory>
#include <iterator>

export module BinaryTree.BinaryTreePreorderIterator;

import BinaryTree.BinaryTreeNode;
import BinaryTree.BinaryTreeIterator;

export namespace dslab {

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreePreorderIterate : public BinaryTreeIterate<T, Node> {
public:
    Node<T>* begin(Node<T>* node) override {
        return node;
    }
    Node<T>* operator()(Node<T>* node) override {
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