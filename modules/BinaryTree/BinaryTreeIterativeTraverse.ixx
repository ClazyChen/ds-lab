export module BinaryTree.Traverse.BinaryTreeIterativeTraverse;

import Framework.Algorithm;
import BinaryTree.BinaryTreeNode;
import BinaryTree.Iterator;
import BinaryTree.Traverse.AbstractBinaryTreeTraverse;
import BinaryTree.Iterator.AbstractBinaryTreeIterate;
import std;

export namespace dslab {

template <typename T, template <typename> typename It>
    requires std::is_base_of_v<AbstractBinaryTreeIterate<T>, It<T>>
class BinaryTreeIterativeTraverse : public AbstractBinaryTreeTraverse<T> {
    It<T> it;
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        for (auto node { it.begin(p) }; node; node = it.next(node)) {
            visit(node);
        }
    }
};

}