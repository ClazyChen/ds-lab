module;
#include <memory>
#include <iostream>

export module BinaryTree.BinaryTreeNode;

import Framework.PointerProxy;

export namespace dslab {

template <typename T>
class BinaryTreeNode;

template <typename T>
using BinaryTreeNodeInst = UniquePointerProxy<BinaryTreeNode<T>>;

template <typename T>
using BinaryTreeNodePos = RawPointerProxy<BinaryTreeNode<T>>;

template <typename T>
using BinaryTreeNodeConstPos = ConstRawPointerProxy<BinaryTreeNode<T>>;

template <typename T>
class BinaryTreeNode {
    T m_data;

    BinaryTreeNodePos<T> m_parent { nullptr };
    BinaryTreeNodeInst<T> m_left { nullptr };
    BinaryTreeNodeInst<T> m_right { nullptr };

    BinaryTreeNodePos<T> childMost() {
        BinaryTreeNodePos<T> p { this };
        while (!p->isLeaf()) {
            if (p->right()) {
                p = p->right();
            } else {
                p = p->left();
            }
        }
        return p;
    }

public:
    BinaryTreeNode() = default;
    BinaryTreeNode(const T& data) : m_data(data) {}
    BinaryTreeNode(T&& data) : m_data(std::move(data)) {}

    virtual ~BinaryTreeNode() {
        auto p { childMost() };
        while (p != this) {
            auto q { p->parent() };
            if (p->isLeftChild()) {
                q->m_left = nullptr;
            } else {
                q->m_right = nullptr;
            }
            p = q->childMost();
        }
    }

    BinaryTreeNode(const BinaryTreeNode&) = delete;
    BinaryTreeNode& operator=(const BinaryTreeNode&) = delete;
    BinaryTreeNode(BinaryTreeNode&&) = delete;
    BinaryTreeNode& operator=(BinaryTreeNode&&) = delete;

    T& data() override { return m_data; }
    const T& data() const { return m_data; }
    BinaryTreeNodeInst<T>& left() { return m_left; }
    const BinaryTreeNodeInst<T>& left() const { return m_left; }
    BinaryTreeNodeInst<T>& right() { return m_right; }
    const BinaryTreeNodeInst<T>& right() const { return m_right; }
    BinaryTreeNodePos<T>& parent() { return m_parent; }
    const BinaryTreeNodePos<T>& parent() const { return m_parent; }

    BinaryTreeNodeInst<T> clone() const {
        auto node { BinaryTreeNodeInst<T>::make(m_data) };
        if (left()) {
            node->left() = left()->clone();
            node->left()->parent() = node;
        }
        if (right()) {
            node->right() = right()->clone();
            node->right()->parent() = node;
        }
        return node;
    }

    bool isRoot() const { return m_parent == nullptr; }
    bool isLeaf() const { return m_left == nullptr && m_right == nullptr; }

    bool isLeftChild() const { return m_parent != nullptr && m_parent->left() == this; }
    bool isRightChild() const { return m_parent != nullptr && m_parent->right() == this; }

    BinaryTreeNodePos<T> leftSibling() {
        if (isRightChild()) {
            return m_parent->left();
        } else {
            return nullptr;
        }
    }
    BinaryTreeNodeConstPos<T> leftSibling() const {
        return const_cast<BinaryTreeNode*>(this)->leftSibling();
    }

    BinaryTreeNodePos<T> rightSibling() {
        if (isLeftChild()) {
            return m_parent->right();
        } else {
            return nullptr;
        }
    }
    BinaryTreeNodeConstPos<T> rightSibling() const {
        return const_cast<BinaryTreeNode*>(this)->rightSibling();
    }

    bool isParent(BinaryTreeNodePos<T> node) const {
        return node != nullptr && node->m_parent == this;
    }

    size_t depth() const {
        size_t d { 0 };
        for (auto p { m_parent }; p != nullptr; p = p->m_parent) {
            ++d;
        }
        return d;
    }

    size_t height() const {
        if (isLeaf()) {
            return 0;
        } else {
            size_t h { 0 };
            if (m_left) {
                h = std::max(h, m_left->height());
            }
            if (m_right) {
                h = std::max(h, m_right->height());
            }
            return h + 1;
        }
    }

    size_t size() const {
        size_t s { 1 };
        if (m_left) {
            s += m_left->size();
        }
        if (m_right) {
            s += m_right->size();
        }
        return s;
    }
};


}