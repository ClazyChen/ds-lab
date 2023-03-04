module;
#include <stdexcept>
#include <variant>
#include <memory>

export module BinaryTree.ThreadedBinaryTree.ThreadedBinaryTreeNode;

import BinaryTree.AbstractBinaryTreeNode;

export namespace dslab {

template <typename T>
class ThreadedBinaryTreeNode : public AbstractBinaryTreeNode<T> {
    using ChildPtr = std::unique_ptr<AbstractBinaryTreeNode<T>>;
    using ThreadedPtr = AbstractBinaryTreeNode<T>*;

    T m_data;
    std::variant<ChildPtr, ThreadedPtr> m_left { };
    std::variant<ChildPtr, ThreadedPtr> m_right { };
public:
    ThreadedBinaryTreeNode() = default;
    ThreadedBinaryTreeNode(const T& data) : m_data(data) {}
    ThreadedBinaryTreeNode(T&& data) : m_data(std::move(data)) {}

    virtual ~ThreadedBinaryTreeNode() = default;
    ThreadedBinaryTreeNode(const ThreadedBinaryTreeNode&) = delete;
    ThreadedBinaryTreeNode& operator=(const ThreadedBinaryTreeNode&) = delete;
    ThreadedBinaryTreeNode(ThreadedBinaryTreeNode&&) = delete;
    ThreadedBinaryTreeNode& operator=(ThreadedBinaryTreeNode&&) = delete;

    T& data() override { return m_data; }
    const T& data() const { return m_data; }
    ChildPtr& left() override {
        if (!std::holds_alternative<ChildPtr>(m_left)) {
            m_left = static_cast<ChildPtr>(nullptr);
        }
        return std::get<ChildPtr>(m_left);
    }
    const ChildPtr& left() const {
        if (!std::holds_alternative<ChildPtr>(m_left)) {
            return nullptr;
        }
        return std::get<ChildPtr>(m_left);
    }
    ChildPtr& right() override {
        if (!std::holds_alternative<ChildPtr>(m_right)) {
            m_right = static_cast<ChildPtr>(nullptr);
        }
        return std::get<ChildPtr>(m_right);
    }
    const ChildPtr& right() const {
        if (!std::holds_alternative<ChildPtr>(m_right)) {
            return nullptr;
        }
        return std::get<ChildPtr>(m_right);
    }

    ThreadedPtr& leftThread() {
        if (!std::holds_alternative<ThreadedPtr>(m_left)) {
            m_left = static_cast<ThreadedPtr>(nullptr);
        }
        return std::get<ThreadedPtr>(m_left);
    }
    const ThreadedPtr& leftThread() const {
        if (!std::holds_alternative<ThreadedPtr>(m_left)) {
            return nullptr;
        }
        return std::get<ThreadedPtr>(m_left);
    }

    ThreadedPtr& rightThread() {
        if (!std::holds_alternative<ThreadedPtr>(m_right)) {
            m_right = static_cast<ThreadedPtr>(nullptr);
        }
        return std::get<ThreadedPtr>(m_right);
    }
    const ThreadedPtr& rightThread() const {
        if (!std::holds_alternative<ThreadedPtr>(m_right)) {
            return nullptr;
        }
        return std::get<ThreadedPtr>(m_right);
    }

    bool hasLeftChild() const {
        return std::holds_alternative<ChildPtr>(m_left);
    }

    bool hasRightChild() const {
        return std::holds_alternative<ChildPtr>(m_right);
    }

    bool hasLeftThread() const {
        return std::holds_alternative<ThreadedPtr>(m_left);
    }

    bool hasRightThread() const {
        return std::holds_alternative<ThreadedPtr>(m_right);
    }

    bool isLeaf() const override {
        return !std::holds_alternative<ChildPtr>(m_left) && !std::holds_alternative<ChildPtr>(m_right);
    }

private:
    [[noreturn]] ThreadedPtr& parent() override {
        throw std::logic_error("ThreadedBinaryTreeNode::parent() is not implemented");
    }

    [[noreturn]] bool isRoot() const override {
        throw std::logic_error("ThreadedBinaryTreeNode::isRoot() is not implemented");
    }

    [[noreturn]] bool isLeftChild() const override {
        throw std::logic_error("ThreadedBinaryTreeNode::isLeftChild() is not implemented");
    }

    [[noreturn]] bool isRightChild() const override {
        throw std::logic_error("ThreadedBinaryTreeNode::isRightChild() is not implemented");
    }

    [[noreturn]] ThreadedPtr leftSibling() override {
        throw std::logic_error("ThreadedBinaryTreeNode::leftSibling() is not implemented");
    }

    [[noreturn]] ThreadedPtr rightSibling() override {
        throw std::logic_error("ThreadedBinaryTreeNode::rightSibling() is not implemented");
    }
};


}