module;
#include <memory>
#include <functional>

export module BinaryTree.BinaryTreeTraverse;

import Framework.Algorithm;
import BinaryTree.AbstractBinaryTreeNode;
import BinaryTree.AbstractBinaryTree;
import BinaryTree.BinaryTreeIterator;

export namespace dslab {

template <typename T>
class BinaryTreeTraverse : public Algorithm<void(const AbstractBinaryTreeNode<T>*, std::function<void(const T&)>)> {};

template <typename T, template <typename> typename It>
    requires std::is_base_of_v<BinaryTreeIterate<T>, It<T>>
class BinaryTreeIterativeTraverse : public BinaryTreeTraverse<T> {
    using Iterator = BinaryTreeConstIterator<T, It>;
public:
    void operator()(const AbstractBinaryTreeNode<T>* node, std::function<void(const T&)> visit) override {
        for (auto it { Iterator::begin(node) }; it != Iterator::end(node); ++it) {
            visit(*it);
        }
    }
};

}