module;
#include <memory>

export module Tree.TreeNode;

import Vector;
import LinearList;

export namespace dslab {

template <typename T, template<typename> typename Linear = DefaultVector>
    requires std::is_base_of_v<AbstractLinearList<T, typename Linear<T>::position_type>, Linear<T>>
class TreeNode {
    T m_data;
    TreeNode* m_parent { nullptr };
    Linear<std::unique_ptr<TreeNode>> m_children {};
public:
    TreeNode() = default;
    TreeNode(const T& data) : m_data(data) {}
    TreeNode(T&& data) : m_data(std::move(data)) {}
    virtual ~TreeNode() {
        m_children.clear();
    }
    TreeNode(const TreeNode&) = delete;
    TreeNode& operator=(const TreeNode&) = delete;
    TreeNode(TreeNode&&) = delete;
    TreeNode& operator=(TreeNode&&) = delete;
    T& data() { return m_data; }
    const T& data() const { return m_data; }
    TreeNode*& parent() { return m_parent; }
    const TreeNode*& parent() const { return m_parent; }
    Linear<std::unique_ptr<TreeNode>>& children() { return m_children; }
    const Linear<std::unique_ptr<TreeNode>>& children() const { return m_children; }

    bool isRoot() const { return m_parent == nullptr; }
    bool isLeaf() const { return m_children.empty(); }
    bool isParent(TreeNode* node) const {
        return node != nullptr && node->m_parent == this;
    }
    bool isChild(TreeNode* node) const {
        return m_parent == node;
    }
    bool isSibling(TreeNode* node) const {
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
};

}