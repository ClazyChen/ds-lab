export module BinaryTree.Traverse.AbstractBinaryTreeTraverse;

import Framework.Algorithm;
import BinaryTree.BinaryTreeNode;
import std;

export namespace dslab {

template <typename T>
class AbstractBinaryTreeTraverse : public Algorithm<void(BinaryTreeNodeConstPos<T>, std::function<void(const T&)>)> {
public:
    virtual void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) = 0;
    void operator()(BinaryTreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        traverse(p, [&visit](BinaryTreeNodeConstPos<T> p) { visit(p->data()); });
    }
};

}