export module AVLTree;

import BinarySearchTree;
import BinaryTree;
import std;

export namespace dslab {

template <typename T, typename Cmp = std::less<T>>
class AVLTree : protected BinarySearchTree<T, Cmp> {
    struct Element {
        static Cmp cmp;
        T value;
        size_t height { 0 };
        Element(T value) : value(value) {}
        operator T() const { return value; }
        bool operator==(const Element& other) const { return value == other.value; }
        bool operator<(const Element& other) const { return cmp(value, other.value); }
    };

    static size_t height(BinaryTreeNodeConstPos<Element> node) { 
        return node ? node->data().height : 0; 
    }

    static size_t bf(BinaryTreeNodeConstPos<Element> node) {
        return height(node->left()) - height(node->right());
    }

    static bool imbalance(BinaryTreeNodeConstPos<Element> node) {
        return std::abs(bf(node)) > 1;
    }

    static void updateHeight(BinaryTreeNodePos<Element> node) {
        node->data().height = std::max(height(node->left()), height(node->right())) + 1
    }

    static void updateHeights(BinaryTreeNodePos<Element>... nodes) {
        (updateHeight(nodes), ...);
    }

    //static void updateHeightAbove(BinaryTreeNodePos<Element> node) {
    //    while (node) {
    //        updateHeight(node);
    //        node = node->parent();
    //    }
    //}

    static BinaryTreeNodePos<Element> tallerChild(BinaryTreeNodeConstPos<Element> node) {
        auto lh { height(node->left()) };
        auto rh { height(node->right()) };
        if (lh > rh) {
            return node->left();
        } else if (lh < rh) {
            return node->right();
        } else {
            return node->isLeftChild() ? node->left() : node->right();
        }
    }

    void rotateRight(BinaryTreeNodePos<Element> p) {
        if (auto v { std::move(p->left()) }; v) {
            auto p { this->replace(p, std::move(v)) };
            this->attachAsLeftChild(p, std::move(v->right()));
            this->attachAsRightChild(v, std::move(p));
        }
    }

    void rotateLeft(BinaryTreeNodePos<Element> p) {
        if (auto v { std::move(p->right()) }; v) {
            auto p { this->replace(p, std::move(v)) };
            this->attachAsRightChild(p, std::move(v->left()));
            this->attachAsLeftChild(v, std::move(p));
        }
    }

    void rebalance(BinaryTreeNodePos<Element> v, BinaryTreeNodePos<Element> p, BinaryTreeNodePos<Element> g) {
        if (p == g->left()) {
            if (v == p->left()) {
                rotateRight(g);
            } else {
                rotateLeft(p);
                rotateRight(g);
            }
        } else {
            if (v == p->right()) {
                rotateLeft(g);
            } else {
                rotateRight(p);
                rotateLeft(g);
            }
        }
        updateHeights(v, p, g);
    }

public:
    using BinaryTree<T>::BinaryTree;

    BinaryTreeNodePos<T> insert(const T& e) {
        auto v0 { this->insert(e) }, v { v0 };
        for (auto p { v->parent() }; p; p = p->parent()) {
            updateHeight(p);
            if (auto g { p->parent() }; g && imbalance(g)) {
                rebalance(v, p, g);
                break;
            }
            v = p;
        }
        return v0;
    }

    BinaryTreeNodePos<T> insert(T&& e) {
        auto v0 { this->insert(std::move(e)) }, v { v0 };
        for (auto p { v->parent() }; p; p = p->parent()) {
            updateHeight(p);
            if (auto g { p->parent() }; g && imbalance(g)) {
                rebalance(v, p, g);
                break;
            }
            v = p;
        }
    }

    bool remove(const T& e) override {
        if (auto p { this->search(e) }; p) {
            if (p->left() && p->right()) {
                auto succ { this->succ(p) };
                p->data() = std::move(succ->data());
                p = succ;
            }
            auto& c { p->left() ? p->left() : p->right() };
            auto g { p->parent() };
            this->replace(p, std::move(c));
            for (; g; g = g->parent()) {
                updateHeight(g);
                if (imbalance(g)) {
                    auto p { tallerChild(g) };
                    auto v { tallerChild(p) };
                    rebalance(v, p, g);
                }
            }
        }
    }
};

}