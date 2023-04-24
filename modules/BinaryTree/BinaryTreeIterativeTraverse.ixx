module;
#include <functional>

export module BinaryTree.Traverse.BinaryTreeIterativeTraverse;

import Framework.Algorithm;
import BinaryTree.BinaryTreeNode;
import BinaryTree.Iterator;
import BinaryTree.Traverse.AbstractBinaryTreeTraverse;

export namespace dslab {

template <typename T, template <typename> typename It>
    requires std::is_base_of_v<AbstractBinaryTreeIterate<T>, It<T>>
class BinaryTreeIterativeTraverse : public AbstractBinaryTreeTraverse<T> {
    using Iterator = BinaryTreeConstIterator<T, It>;
public:
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        for (auto it { Iterator::begin(p) }; it != Iterator::end(p); ++it) {
            visit(*it);
        }
    }
};

}