export module Tree.TreeNode;

import Framework.PointerProxy;
import Vector;
import LinearList;
import std;

export namespace dslab {

template <typename T>
class TreeNode;

template <typename T>
using TreeNodeInst = UniquePointerProxy<TreeNode<T>>;

template <typename T>
using TreeNodePos = RawPointerProxy<TreeNode<T>>;

template <typename T>
using TreeNodeConstPos = ConstRawPointerProxy<TreeNode<T>>;

template <typename T>
class TreeNode {
    T m_data;
    TreeNodePos<T> m_parent { nullptr };
    Vector<TreeNodeInst<T>> m_children {};

    TreeNodePos<T> rightMost() {
        TreeNodePos<T> p { this };
        while (!p->isLeaf()) {
            p = p->children().back();
        }
        return p;
    }
public:
    TreeNode() = default;
    TreeNode(const T& data) : m_data(data) {}
    TreeNode(T&& data) : m_data(std::move(data)) {}
    virtual ~TreeNode() {
        auto p { rightMost() };
        while (p != this) {
            auto q { p->parent() };
            q->children().pop_back();
            p = q->rightMost();
        }
    }
    TreeNode(const TreeNode&) = delete;
    TreeNode& operator=(const TreeNode&) = delete;
    TreeNode(TreeNode&&) = delete;
    TreeNode& operator=(TreeNode&&) = delete;
    T& data() { return m_data; }
    const T& data() const { return m_data; }
    TreeNodePos<T>& parent() { return m_parent; }
    const TreeNodePos<T>& parent() const { return m_parent; }
    Vector<TreeNodeInst<T>>& children() { return m_children; }
    const Vector<TreeNodeInst<T>>& children() const { return m_children; }
    TreeNodePos<T> child(size_t index) { return m_children[index]; }
    TreeNodeConstPos<T> child(size_t index) const { return m_children[index]; }

    TreeNodeInst<T> clone() const {
        auto node { TreeNodeInst<T>::make(m_data) };
        for (const auto& child : m_children) {
            node->children().push_back(child->clone());
            node->children().back()->parent() = node;
        }
        return node;
    }

    bool isRoot() const { return m_parent == nullptr; }
    bool isLeaf() const { return m_children.empty(); }
    bool isParent(TreeNodePos<T> node) const {
        return node != nullptr && node->m_parent == this;
    }
    bool isChild(TreeNodePos<T> node) const {
        return m_parent == node;
    }
    bool isSibling(TreeNodePos<T> node) const {
        return m_parent == node->m_parent;
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
        }
        size_t h { 0 };
        for (const auto& child : m_children) {
            h = std::max(h, child->height());
        }
        return h + 1;
    }

    size_t size() const {
        size_t s { 1 };
        for (const auto& child : m_children) {
            s += child->size();
        }
        return s;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const TreeNode<T>& node) {
    os << node.data();
    if (!node.isLeaf()) {
        os << "(";
        for (const auto& child : node.children()) {
            os << *child << ", ";
        }
        os << "\b\b)";
    }
    return os;
}

}