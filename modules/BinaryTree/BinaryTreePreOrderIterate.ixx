export module BinaryTree.Iterator.BinaryTreePreOrderIterate;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Iterator.AbstractBinaryTreeIterate;
import std;

export namespace dslab {

template <typename T>
class BinaryTreePreOrderIterate : public AbstractBinaryTreeIterate<T> {
public:
    BinaryTreeNodeConstPos<T> begin(BinaryTreeNodeConstPos<T> p) override {
        return p;
    }
    BinaryTreeNodeConstPos<T> next(BinaryTreeNodeConstPos<T> p) override {
        if (p == nullptr) {
            return nullptr;
        }
        if (auto& left { p->left() }; left) {
            return left;
        }
        if (auto& right { p->right() }; right) {
            return right;
        }
        while (p) {
            if (auto sib { p->rightSibling() }; sib) {
                return sib;
            }
            p = p->parent();
        }
        return nullptr;
    }
};

}