module;
#include <memory>

export module BinaryTree.BinaryTreeNode;

import Framework.PointerProxy;

export namespace dslab {

template <typename T>
class BinaryTreeNode {
    T m_data;

    using BinaryTreeNodeProxy = UniquePointerProxy<BinaryTreeNode<T>>;
    using BinaryTreeNodeRawProxy = RawPointerProxy<BinaryTreeNode<T>>;
    
    BinaryTreeNodeRawProxy m_parent {};
    BinaryTreeNodeProxy m_left {};
    BinaryTreeNodeProxy m_right {};

public:
    BinaryTreeNode() = default;
    BinaryTreeNode(const T& data) : m_data(data) {}
    BinaryTreeNode(T&& data) : m_data(std::move(data)) {}

    virtual ~BinaryTreeNode() = default;
    BinaryTreeNode(const BinaryTreeNode&) = delete;
    BinaryTreeNode& operator=(const BinaryTreeNode&) = delete;
    BinaryTreeNode(BinaryTreeNode&&) = delete;
    BinaryTreeNode& operator=(BinaryTreeNode&&) = delete;

    T& data() override { return m_data; }
    const T& data() const { return m_data; }
    BinaryTreeNodeProxy& left() override { return m_left; }
    const BinaryTreeNodeProxy& left() const { return m_left; }
    BinaryTreeNodeProxy& right() override { return m_right; }
    const BinaryTreeNodeProxy& right() const { return m_right; }
    BinaryTreeNodeRawProxy& parent() override { return m_parent; }
    const BinaryTreeNodeRawProxy& parent() const { return m_parent; }

    bool isRoot() const override { return m_parent == nullptr; }
    bool isLeaf() const override { return m_left == nullptr && m_right == nullptr; }

    bool isLeftChild() const override { return m_parent != nullptr && m_parent->left() == this; }
    bool isRightChild() const override { return m_parent != nullptr && m_parent->right() == this; }

    BinaryTreeNode* leftSibling() override {
        if (isRightChild()) {
            return m_parent->left();
        } else {
            return nullptr;
        }
    }
    const BinaryTreeNode* leftSibling() const {
        return const_cast<BinaryTreeNode*>(this)->leftSibling();
    }

    BinaryTreeNode* rightSibling() override {
        if (isLeftChild()) {
            return m_parent->right();
        } else {
            return nullptr;
        }
    }
    const BinaryTreeNode* rightSibling() const {
        return const_cast<BinaryTreeNode*>(this)->rightSibling();
    }
};


}