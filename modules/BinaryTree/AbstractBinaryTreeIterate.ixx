export module BinaryTree.Iterator.AbstractBinaryTreeIterate;

import BinaryTree.BinaryTreeNode;

export namespace dslab {

template <typename T>
class AbstractBinaryTreeIterate {
public:
    virtual BinaryTreeNodeConstPos<T> begin(BinaryTreeNodeConstPos<T> p) = 0;
    virtual BinaryTreeNodeConstPos<T> end(BinaryTreeNodeConstPos<T> p) {
        return nullptr;
    }
    virtual BinaryTreeNodeConstPos<T> next(BinaryTreeNodeConstPos<T> p) = 0;
};

}