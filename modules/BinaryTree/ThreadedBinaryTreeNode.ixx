module;
#include <stdexcept>
#include <variant>
#include <memory>

export module ThreadedBinaryTree.ThreadedBinaryTreeNode;

import Framework.PointerProxy;

export namespace dslab {

template <typename T>
class ThreadedBinaryTreeNode;

template <typename T>
using ThreadedBinaryTreeNodeInst = UniquePointerProxy<ThreadedBinaryTreeNode<T>>;

template <typename T>
using ThreadedBinaryTreeNodePos = RawPointerProxy<ThreadedBinaryTreeNode<T>>;

template <typename T>
using ThreadedBinaryTreeNodeConstPos = ConstRawPointerProxy<ThreadedBinaryTreeNode<T>>;

template <typename T>
class ThreadedBinaryTreeNode {
    T m_data;

    std::variant<ThreadedBinaryTreeNodeInst<T>, ThreadedBinaryTreeNodePos<T>> m_left { };
    std::variant<ThreadedBinaryTreeNodeInst<T>, ThreadedBinaryTreeNodePos<T>> m_right { };

public:
    ThreadedBinaryTreeNode() = default;
    ThreadedBinaryTreeNode(const T& data) : m_data(data) {}
    ThreadedBinaryTreeNode(T&& data) : m_data(std::move(data)) {}

    virtual ~ThreadedBinaryTreeNode() {
        if (m_left.index() == 0) {
            std::get<ThreadedBinaryTreeNodeInst<T>>(m_left)->m_right = nullptr;
        }
        if (m_right.index() == 0) {
            std::get<ThreadedBinaryTreeNodeInst<T>>(m_right)->m_left = nullptr;
        }
    }

    ThreadedBinaryTreeNode(const ThreadedBinaryTreeNode&) = delete;
    ThreadedBinaryTreeNode& operator=(const ThreadedBinaryTreeNode&) = delete;
    ThreadedBinaryTreeNode(ThreadedBinaryTreeNode&&) = delete;
    ThreadedBinaryTreeNode& operator=(ThreadedBinaryTreeNode&&) = delete;

    T& data() { return m_data; }
    const T& data() const { return m_data; }
    ThreadedBinaryTreeNodeInst<T>& left() {
        return std::get<ThreadedBinaryTreeNodeInst<T>>(m_left);
    }
    const ThreadedBinaryTreeNodeInst<T>& left() const {
        return std::get<ThreadedBinaryTreeNodeInst<T>>(m_left);
    }
    ThreadedBinaryTreeNodeInst<T>& right() {
        return std::get<ThreadedBinaryTreeNodeInst<T>>(m_right);
    }
    const ThreadedBinaryTreeNodeInst<T>& right() const {
        return std::get<ThreadedBinaryTreeNodeInst<T>>(m_right);
    }

    bool hasLeft() const {
        return m_left.index() == 0;
    }
    bool hasRight() const {
        return m_right.index() == 0;
    }
    bool hasLeftThread() const {
        return m_left.index() == 1;
    }
    bool hasRightThread() const {
        return m_right.index() == 1;
    }

    bool isLeaf() const {
        return !hasLeft() && !hasRight();
    }
};


}