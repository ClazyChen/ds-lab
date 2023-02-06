module;
#include <memory>
#include <iterator>

export module BinaryTree.BinaryTreePostorderIterator;

import BinaryTree.BinaryTreeNode;
import BinaryTree.BinaryTreeIterator;

export namespace dslab {

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreePostorderIterate : public BinaryTreeIterate<T, Node> {
    Node<T>* findLeftMost(Node<T>* node) {
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
    Node<T>* begin(Node<T>* node) override {
        return findLeftMost(node);
    }
    Node<T>* operator()(Node<T>* node) override {
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