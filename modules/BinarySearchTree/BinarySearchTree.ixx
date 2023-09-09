export module BinarySearchTree;

import BinaryTree;
import Vector;
import BinarySearchTree.AbstractBinarySearchTree;
import std;

export namespace dslab {

template <typename T, typename Cmp = std::less<T>>
class BinarySearchTree : public AbstractBinarySearchTree<T, Cmp> {
    BinaryTreeNodeConstPos<T> searchFirst(const T& e) const {
        BinaryTreeNodeConstPos<T> cur { nullptr };
        for (auto p { this->root() }; p; ) {
            if (auto& x { p->data() }; e == x) {
                cur = p;
                p = p->left();
            } else if (this->cmp(e, x)) {
                p = p->left();
            } else {
                p = p->right();
            }
        }
        return cur;
    }

public:
    BinaryTreeNodePos<T> insert(const T& e) override {
        for (auto p { this->root() }; p; ) {
            if (auto& x { p->data() }; this->cmp(e, x)) {
                if (p->left()) {
                    p = p->left();
                } else {
                    return this->insertAsLeftChild(p, e);
                }
            } else {
                if (p->right()) {
                    p = p->right();
                } else {
                    return this->insertAsRightChild(p, e);
                }
            }
        }
        return this->insertAsRoot(e);
    }

    BinaryTreeNodePos<T> insert(T&& e) override {
        for (auto p { this->root() }; p; ) {
            if (auto& x { p->data() }; this->cmp(e, x)) {
                if (p->left()) {
                    p = p->left();
                } else {
                    return this->insertAsLeftChild(p, std::move(e));
                }
            } else {
                if (p->right()) {
                    p = p->right();
                } else {
                    return this->insertAsRightChild(p, std::move(e));
                }
            }
        }
    }

    BinaryTreeNodePos<T> search(const T& e) {
        for (auto p { this->root() }; p; ) {
            if (auto& x { p->data() }; e == x) {
                return p;
            } else if (this->cmp(e, x)) {
                p = p->left();
            } else {
                p = p->right();
            }
        }
        return nullptr;
    }

    BinaryTreeNodeConstPos<T> search(const T& e) const override {
        return const_cast<BinarySearchTree*>(this)->search(e);
    }

    bool remove(const T& e) override {
        static size_t cnt { 0 };
        if (auto p { search(e) }; p) {
            if (p->left() && p->right()) {
                auto alt { cnt++ % 2 ? this->pred(p) : this->succ(p) };
                p->data() = std::move(alt->data());
                p = alt;
            }
            auto& c { p->left() ? p->left() : p->right() };
            this->replace(p, std::move(c));
            return true;
        }
        return false;
    }

    Vector<BinaryTreeNodeConstPos<T>> searchAll(const T& e) const override {
        Vector<BinaryTreeNodeConstPos<T>> v;
        if (auto first { searchFirst(e) }; first) {
            for (auto p { first }; p; p = this->succ(p)) {
                if (e == p->data()) {
                    v.push_back(p);
                } else {
                    break;
                }
            }
        }
        return v;
    }
};

}