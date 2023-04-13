module;
#include <memory>

export module Tree;

export import Tree.TreeNode;
export import Tree.AbstractTree;

export namespace dslab {

template <typename T>
class Tree : public AbstractTree<T> {
    TreeNodeInst<T> m_root { nullptr };
public:
    TreeNodePos<T> root() override { return m_root; }
    TreeNodeConstPos<T> root() const override { return m_root; }
    size_t size() const override { return m_root->size(); }

    Tree() = default;
    Tree(const Tree& t) = delete;
    Tree& operator=(const Tree& t) = delete;
    Tree(Tree&& t) = default;
    Tree& operator=(Tree&& t) = default;
    virtual ~Tree() = default;

    Tree(TreeNodeInst<T>&& root) : m_root(std::move(root)) {}
    Tree& operator=(TreeNodeInst<T>&& root) {
        m_root = std::move(root);
        return *this;
    }

    operator TreeNodeInst<T>&&() { return std::move(m_root); }

    void clear() override {
        m_root.reset();
    }

    TreeNodePos<T> insertAsChild(TreeNodePos<T> p, const T& e, size_t index = -1) override {
        auto node { TreeNodeInst<T>::make(e) };
        node->parent() = p;
        if (index == -1) {
            index = p->children().size();
        }
        p->children().insert(index, std::move(node));
        return p->children()[index];
    }

    TreeNodePos<T> insertAsChild(TreeNodePos<T> p, T&& e, size_t index = -1) override {
        auto node { TreeNodeInst<T>::make(std::move(e)) };
        node->parent() = p;
        if (index == -1) {
            index = p->children().size();
        }
        p->children().insert(index, std::move(node));
        return p->children()[index];
    }

    TreeNodePos<T> insertAsRoot(const T& e) override {
        auto node { TreeNodeInst<T>::make(e) };
        m_root = std::move(node);
        return m_root;
    }

    TreeNodePos<T> insertAsRoot(T&& e) override {
        auto node { TreeNodeInst<T>::make(std::move(e)) };
        m_root = std::move(node);
        return m_root;
    }

    TreeNodePos<T> attachAsChild(TreeNodePos<T> p, TreeNodeInst<T>&& node, size_t index = -1) override {
        node->parent() = p;
        if (index == -1) {
            index = p->children().size();
        }
        p->children().insert(index, std::move(node));
        return p->children()[index];
    }

    T remove(TreeNodePos<T> p) override {
        if (p == m_root) {
            return std::move(m_root)->data();
        } else {
            auto& children { p->parent()->children() };
            auto index { 0 };
            while (children[index] != p) {
                ++index;
            }
            return children.remove(index)->data();
        }
    }

    TreeNodeInst<T> detach(TreeNodePos<T> p) override {
        if (p == m_root) {
            return std::move(m_root);
        } else {
            auto& children { p->parent()->children() };
            auto index { 0 };
            while (children[index] != p) {
                ++index;
            }
            return children.remove(index);
        }
    }
};

}