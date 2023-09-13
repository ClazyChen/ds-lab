export module BinaryTree.Iterator.BinaryTreeInOrderIterate;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Iterator.AbstractBinaryTreeIterate;
import std;

export namespace dslab {

template <typename T>
class BinaryTreeInOrderIterate : public AbstractBinaryTreeIterate<T> {
    BinaryTreeNodeConstPos<T> leftMost(BinaryTreeNodeConstPos<T> p) {
        while (p->left() != nullptr) {
            p = p->left();
        }
        return p;
    }
public:
    BinaryTreeNodeConstPos<T> begin(BinaryTreeNodeConstPos<T> p) override {
        return leftMost(p);
    }
    BinaryTreeNodeConstPos<T> next(BinaryTreeNodeConstPos<T> p) override {
        if (p == nullptr) {
            return nullptr;
        }
        if (auto& right { p->right() }; right) {
            return leftMost(right);
        } else {
            while (p->isRightChild()) {
                p = p->parent();
            }
            return p->parent();
        }
    }
    std::string type_name() const override {
        return "InOrder";
    }
};

template <typename T>
class BinaryTreeInOrderReverseIterate : public AbstractBinaryTreeIterate<T> {
    BinaryTreeNodeConstPos<T> rightMost(BinaryTreeNodeConstPos<T> p) {
        while (p->right() != nullptr) {
            p = p->right();
        }
        return p;
    }
public:
    BinaryTreeNodeConstPos<T> begin(BinaryTreeNodeConstPos<T> p) override {
        return rightMost(p);
    }
    BinaryTreeNodeConstPos<T> next(BinaryTreeNodeConstPos<T> p) override {
        if (p == nullptr) {
            return nullptr;
        }
        if (auto& left { p->left() }; left) {
            return rightMost(left);
        } else {
            while (p->isLeftChild()) {
                p = p->parent();
            }
            return p->parent();
        }
    }
    std::string type_name() const override {
        return "InOrder / Reverse";
    }
};

}