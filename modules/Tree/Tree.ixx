export module Tree;

export import Tree.TreeNode;
export import Tree.AbstractTree;
export import Tree.Traverse;

import Tree.Traverse.AbstractTreeTraverse;
import Stack;
import std;

export namespace dslab {

template <typename T>
class Tree : public AbstractTree<T> {
protected:
    TreeNodeInst<T> m_root { nullptr };

    TreeNodeInst<T> clone() const {
        if (m_root == nullptr) {
            return nullptr;
        } 
        auto r { TreeNodeInst<T>::make(m_root->data()) };
        Stack<TreeNodePos<T>> S { r };
        traverseNodes<TreePreOrderTraverseIterative>([&S](auto node) {
            auto cur { S.pop() };
            cur->children().resize(node->children().size());
            for (size_t i { node->children().size() }; i > 0; --i) {
                auto child { TreeNodeInst<T>::make(node->children()[i - 1]->data()) };
                S.push(child);
                child->parent() = cur;
                cur->children()[i - 1] = std::move(child);
            }
        });
        return r;
    }

public:
    TreeNodePos<T> root() override { return m_root; }
    TreeNodeConstPos<T> root() const override { return m_root; }
    
    // WARNING: O(n)
    size_t size() const override {
        size_t result { 0 };
        traverse<TreePreOrderTraverseIterative>([&result](const T&) { ++result; });
        return result;
    }
    bool empty() const override {
        return m_root == nullptr;
    }

    Tree() = default;
    Tree(const Tree& t) {
        m_root = t.clone();
    }
    Tree& operator=(const Tree& t) {
        m_root = t.clone();
        return *this;
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
        traverseNodes<TreePreOrderTraverse>([&node, &e](auto p) {
            if (p->data() == e) {
                node = p;
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
            size_t index { 0 };
            while (children[index] != p) {
                ++index;
            }
            return children.remove(index);
        }
    }

    std::string type_name() const override {
        return "Tree";
    }

    template <template <typename> typename Trav>
        requires std::is_base_of_v<AbstractTreeTraverse<T>, Trav<T>>
    void traverse(std::function<void(const T&)> visit) const {
        return Trav<T> {} (m_root, visit);
    }

    template <template <typename> typename Trav>
        requires std::is_base_of_v<AbstractTreeTraverse<T>, Trav<T>>
    void traverseNodes(std::function<void(TreeNodeConstPos<T>)> visit) const {
        return Trav<T> {} .traverse(m_root, visit);
    }

};

}