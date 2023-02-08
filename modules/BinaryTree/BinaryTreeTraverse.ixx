module;
#include <memory>
#include <functional>

export module BinaryTree.BinaryTreeTraverse;

import Framework.Algorithm;
import BinaryTree.BinaryTreeNode;
import BinaryTree.AbstractBinaryTree;
import BinaryTree.BinaryTreeIterator;

export namespace dslab {

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreeTraverse : public Algorithm<void(const Node<T>*, std::function<void(const T&)>)> {};

template <typename T, template <typename, template <typename> typename> typename It, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>&& std::is_base_of_v<BinaryTreeIterate<T, Node>, It<T, Node>>
class BinaryTreeIterativeTraverse : public BinaryTreeTraverse<T, Node> {
    using Iterator = BinaryTreeConstIterator<T, It, Node>;
public:
    void operator()(const Node<T>* node, std::function<void(const T&)> visit) override {
        for (auto it { Iterator::begin(node) }; it != Iterator::end(node); ++it) {
            visit(*it);
        }
    }
};

}