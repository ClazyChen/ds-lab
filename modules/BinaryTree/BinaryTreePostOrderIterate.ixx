export module BinaryTree.Iterator.BinaryTreePostOrderIterate;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Iterator.AbstractBinaryTreeIterate;
import std;

export namespace dslab {

template <typename T>
class BinaryTreePostOrderIterate : public AbstractBinaryTreeIterate<T> {
    BinaryTreeNodeConstPos<T> childMost(BinaryTreeNodeConstPos<T> p) {
        while (!p->isLeaf()) {
            if (p->left()) {
                p = p->left();
            } else {
                p = p->right();
            }
        }
        return p;
    }
public:
    BinaryTreeNodeConstPos<T> begin(BinaryTreeNodeConstPos<T> p) override {
        return childMost(p);
    }
    BinaryTreeNodeConstPos<T> next(BinaryTreeNodeConstPos<T> p) override {
        if (p == nullptr) {
            return nullptr;
        }
        if (auto sib { p->rightSibling() }; sib) {
            return childMost(sib);
        } else {
            return p->parent();
        }
    }
    std::string type_name() const override {
        return "PostOrder";
    }
};

}