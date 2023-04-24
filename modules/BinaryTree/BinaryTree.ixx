module;
#include <functional>
#include <memory>

export module BinaryTree;

export import BinaryTree.BinaryTreeNode;
export import BinaryTree.AbstractBinaryTree;
export import BinaryTree.Traverse;
export import BinaryTree.Iterator;

import BinaryTree.Traverse.AbstractBinaryTreeTraverse;

export namespace dslab {

template <typename T>
class BinaryTree : public AbstractBinaryTree<T> {
    BinaryTreeNodeInst<T> m_root { nullptr };
public:
    BinaryTreeNodePos<T> root() override { return m_root; }
    BinaryTreeNodeConstPos<T> root() const override { return m_root; }
    size_t size() const override {
        size_t result { 0 };
        traverse<BinaryTreePreOrderTraverseSemilinear>([&result](auto) { ++result; });
        return result;
    }

    BinaryTree() = default;
    BinaryTree(const BinaryTree& t) {
        m_root = t.m_root->clone();
    }
    BinaryTree& operator=(const BinaryTree& t) {
        m_root = t.m_root->clone();
        return *this;
    }
    BinaryTree(BinaryTree&& t) = default;
    BinaryTree& operator=(BinaryTree&& t) = default;
    virtual ~BinaryTree() = default;

    BinaryTree(BinaryTreeNodeInst<T>&& root) : m_root(std::move(root)) {}
    BinaryTree& operator=(BinaryTreeNodeInst<T>&& root) {
        m_root = std::move(root);
        return *this;
    }
    
    BinaryTree(const T& e) {
        m_root = BinaryTreeNodeInst<T>::make(e);
    }

    BinaryTree(T&& e) {
        m_root = BinaryTreeNodeInst<T>::make(std::move(e));
    }

    operator BinaryTreeNodeInst<T>&& () { return std::move(m_root); }

    void clear() override {
        m_root.reset();
    }

    BinaryTreeNodePos<T> insertAsLeftChild(BinaryTreeNodePos<T> p, const T& e) override {
        return attachAsLeftChild(p, BinaryTreeNodeInst<T>::make(e));
    }

    BinaryTreeNodePos<T> insertAsRightChild(BinaryTreeNodePos<T> p, const T& e) override {
        return attachAsRightChild(p, BinaryTreeNodeInst<T>::make(e));
    }

    BinaryTreeNodePos<T> insertAsLeftChild(BinaryTreeNodePos<T> p, T&& e) override {
        return attachAsLeftChild(p, BinaryTreeNodeInst<T>::make(std::move(e)));
    }

    BinaryTreeNodePos<T> insertAsRightChild(BinaryTreeNodePos<T> p, T&& e) override {
        return attachAsRightChild(p, BinaryTreeNodeInst<T>::make(std::move(e)));
    }

    BinaryTreeNodePos<T> insertAsRoot(const T& e) override {
        m_root = BinaryTreeNodeInst<T>::make(e);
        return m_root;
    }

    BinaryTreeNodePos<T> insertAsRoot(T&& e) override {
        m_root = BinaryTreeNodeInst<T>::make(std::move(e));
        return m_root;
    }

    BinaryTreeNodePos<T> attachAsLeftChild(BinaryTreeNodePos<T> p, BinaryTreeNodeInst<T>&& st) override {
        p->leftChild = std::move(st);
        if (p->leftChild) {
            p->leftChild->parent = p;
        }
        return p->leftChild;
    }

    BinaryTreeNodePos<T> attachAsRightChild(BinaryTreeNodePos<T> p, BinaryTreeNodeInst<T>&& st) override {
        p->rightChild = std::move(st);
        if (p->rightChild) {
            p->rightChild->parent = p;
        }
        return p->rightChild;
    }

    BinaryTreeNodePos<T> find(const T& e) const override {
        BinaryTreeNodePos<T> node { nullptr };
        // ...
        return node;
    }

    T remove(BinaryTreeNodePos<T> p) override {
        return std::move(detach(p)->data());
    }

    BinaryTreeNodeInst<T> detach(BinaryTreeNodePos<T> p) override {
        if (p == m_root) {
            return std::move(m_root);
        } else {
            if (p->isLeftChild()) {
                return std::move(p->parent()->left());
            } else {
                return std::move(p->parent()->right());
            }
        }
    }

    template <template <typename> typename Trav>
        requires std::is_base_of_v<AbstractBinaryTreeTraverse<T>, Trav<T>>
    void traverse(std::function<void(const T&)> visit) const {
        return Trav<T> {} (m_root, visit);
    }
};

}