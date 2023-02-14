module;
#include <memory>

export module BinaryTree.BinaryTreeNode;

import BinaryTree.AbstractBinaryTreeNode;

export namespace dslab {

template <typename T>
class BinaryTreeNode : public AbstractBinaryTreeNode<T> {
    T m_data;
    std::unique_ptr<AbstractBinaryTreeNode<T>> m_left { nullptr };
    std::unique_ptr<AbstractBinaryTreeNode<T>> m_right { nullptr };
    AbstractBinaryTreeNode<T>* m_parent { nullptr };
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
    std::unique_ptr<AbstractBinaryTreeNode<T>>& left() override { return m_left; }
    const std::unique_ptr<AbstractBinaryTreeNode<T>>& left() const { return m_left; }
    std::unique_ptr<AbstractBinaryTreeNode<T>>& right() override { return m_right; }
    const std::unique_ptr<AbstractBinaryTreeNode<T>>& right() const { return m_right; }
    AbstractBinaryTreeNode<T>*& parent() override { return m_parent; }
    const AbstractBinaryTreeNode<T>* parent() const { return m_parent; }

    bool isRoot() const override { return m_parent == nullptr; }
    bool isLeaf() const override { return m_left == nullptr && m_right == nullptr; }

    bool isLeftChild() const override { return m_parent != nullptr && m_parent->left().get() == this; }
    bool isRightChild() const override { return m_parent != nullptr && m_parent->right().get() == this; }

    AbstractBinaryTreeNode<T>* leftSibling() override {
        if (isRightChild()) {
            return m_parent->left().get();
        } else {
            return nullptr;
        }
    }
    const AbstractBinaryTreeNode<T>* leftSibling() const {
        return const_cast<BinaryTreeNode*>(this)->leftSibling();
    }

    AbstractBinaryTreeNode<T>* rightSibling() override {
        if (isLeftChild()) {
            return m_parent->right().get();
        } else {
            return nullptr;
        }
    }
    const AbstractBinaryTreeNode<T>* rightSibling() const {
        return const_cast<BinaryTreeNode*>(this)->rightSibling();
    }
};


}