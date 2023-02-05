module;
#include <memory>
#include <iterator>

export module BinaryTree.BinaryTreeInorderIterator;

import BinaryTree.BinaryTreeNode;
import BinaryTree.BinaryTreeIterator;

export namespace dslab {

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreeInorderIterate : public BinaryTreeIterate<T, Node> {
    Node<T>* findLeft(Node<T>* node) {
        while (node->left() != nullptr) {
            node = node->left().get();
        }
        return node;
    }
public:
    Node<T>* begin(Node<T>* node) override {
        return findLeft(node);
    }
    Node<T>* operator()(Node<T>* node) override {
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