module;
#include <memory>

export module BinaryTree.BinaryTreeNode;

export namespace dslab {

template <typename T>
class BinaryTreeNode {
    T m_data;
    std::unique_ptr<BinaryTreeNode> m_left { nullptr };
    std::unique_ptr<BinaryTreeNode> m_right { nullptr };
    BinaryTreeNode* m_parent { nullptr };
public:
    BinaryTreeNode() = default;
    BinaryTreeNode(const T& data) : m_data(data) {}
    BinaryTreeNode(T&& data) : m_data(std::move(data)) {}

    virtual ~BinaryTreeNode() = default;
    BinaryTreeNode(const BinaryTreeNode&) = delete;
    BinaryTreeNode& operator=(const BinaryTreeNode&) = delete;
    BinaryTreeNode(BinaryTreeNode&&) = delete;
    BinaryTreeNode& operator=(BinaryTreeNode&&) = delete;

    T& data() { return m_data; }
    const T& data() const { return m_data; }
    std::unique_ptr<BinaryTreeNode>& left() { return m_left; }
    const std::unique_ptr<BinaryTreeNode>& left() const { return m_left; }
    std::unique_ptr<BinaryTreeNode>& right() { return m_right; }
    const std::unique_ptr<BinaryTreeNode>& right() const { return m_right; }
    BinaryTreeNode*& parent() { return m_parent; }
    const BinaryTreeNode*& parent() const { return m_parent; }

    virtual bool isRoot() const { return m_parent == nullptr; }
    virtual bool isLeaf() const { return m_left == nullptr && m_right == nullptr; }

    virtual bool isLeftChild() const { return m_parent != nullptr && m_parent->m_left.get() == this; }
    virtual bool isRightChild() const { return m_parent != nullptr && m_parent->m_right.get() == this; }

    virtual BinaryTreeNode* leftSibling() const {
        if (isRightChild()) {
            return m_parent->m_left.get();
        } else {
            return nullptr;
        }
    }

    virtual BinaryTreeNode* rightSibling() const {
        if (isLeftChild()) {
            return m_parent->m_right.get();
        } else {
            return nullptr;
        }
    }
};


}