module;
#include <memory>

export module Tree;

export import Tree.TreeNode;
export import Tree.AbstractTree;
export import Tree.Traverse;

export namespace dslab {

template <typename T>
class Tree : public AbstractTree<T> {
    TreeNodeInst<T> m_root { nullptr };
public:
    TreeNodePos<T> root() override { return m_root; }
    TreeNodeConstPos<T> root() const override { return m_root; }
    size_t size() const override {
        if (!m_root) {
            return 0;
        }
        return m_root->size();
    }

    Tree() = default;
    Tree(const Tree& t) {
        m_root = t.m_root->clone();
    }
    Tree& operator=(const Tree& t) {
        m_root = t.m_root->clone();
    }
    Tree(Tree&& t) = default;
    Tree& operator=(Tree&& t) = default;
    virtual ~Tree() = default;

    Tree(TreeNodeInst<T>&& root) : m_root(std::move(root)) {}
    Tree& operator=(TreeNodeInst<T>&& root) {
        m_root = std::move(root);
        return *this;
    }

    Tree(const T& e) {
        m_root = TreeNodeInst<T>::make(e);
    }

    Tree(T&& e) {
        m_root = TreeNodeInst<T>::make(std::move(e));
    }

    operator TreeNodeInst<T>&&() { return std::move(m_root); }

    void clear() override {
        m_root.reset();
    }

    TreeNodePos<T> insertAsChild(TreeNodePos<T> p, const T& e, size_t index = -1) override {
        return attachAsChild(p, TreeNodeInst<T>::make(e), index);
    }

    TreeNodePos<T> insertAsChild(TreeNodePos<T> p, T&& e, size_t index = -1) override {
        return attachAsChild(p, TreeNodeInst<T>::make(std::move(e)), index);
    }

    TreeNodePos<T> insertAsRoot(const T& e) override {
        m_root = TreeNodeInst<T>::make(e);
        return m_root;
    }

    TreeNodePos<T> insertAsRoot(T&& e) override {
        m_root = TreeNodeInst<T>::make(std::move(e));
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

    TreeNodePos<T> find(const T& e) const override {
        TreeNodePos<T> node { nullptr };
        TreePreOrderTraverse<T> traverse {};
        traverse(m_root, [&node, &e, &traverse](const T& f) {
            if (e == f) {
                node = traverse.current();
            }
        });
        return node;
    }

    T remove(TreeNodePos<T> p) override {
        return std::move(detach(p)->data());
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