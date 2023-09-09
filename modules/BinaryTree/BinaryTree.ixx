export module BinaryTree;

export import BinaryTree.BinaryTreeNode;
export import BinaryTree.AbstractBinaryTree;
export import BinaryTree.Traverse;
export import BinaryTree.Iterator;

import BinaryTree.Traverse.AbstractBinaryTreeTraverse;
import Stack;
import std;

export namespace dslab {

template <typename T>
class BinaryTree : public AbstractBinaryTree<T> {
protected:
    BinaryTreeNodeInst<T> m_root { nullptr };
    BinaryTreeNodeInst<T> clone() const {
        if (m_root == nullptr) {
            return nullptr;
        }
        auto r { BinaryTreeNodeInst<T>::make(m_root->data()) };
        Stack<BinaryTreeNodePos<T>> S { r };
        traverseNodes<BinaryTreePreOrderTraverseSemilinear>([&S](auto node) {
            auto cur { S.pop() };
            if (node->right()) {
                auto child { BinaryTreeNodeInst<T>::make(node->right()) };
                S.push(child);
                child->parent() = cur;
                cur->right() = std::move(child);
            }
            if (node->left()) {
                auto child { BinaryTreeNodeInst<T>::make(node->left()) };
                S.push(child);
                child->parent() = cur;
                cur->left() = std::move(child);
            }
        });
        return r;
    }

    // 将另一个节点替换当前节点，返回被换下的节点
    BinaryTreeNodeInst<T> replace(BinaryTreeNodePos<T> node, BinaryTreeNodeInst<T>&& other) {
        if (auto p { node->parent() }; p) {
            if (other) other->parent() = p;
            if (p->left() == node) {
                std::swap(p->left(), other);
            } else {
                std::swap(p->right(), other);
            }
        } else {
            std::swap(m_root, other);
        }
        return std::move(other);
    }

public:
    BinaryTreeNodePos<T> root() override { return m_root; }
    BinaryTreeNodeConstPos<T> root() const override { return m_root; }
    
    // WARNING: O(n)
    size_t size() const override {
        size_t result { 0 };
        traverse<BinaryTreePreOrderTraverseSemilinear>([&result](auto) { ++result; });
        return result;
    }
    bool empty() const override {
        return m_root == nullptr;
    }

    BinaryTree() = default;
    BinaryTree(const BinaryTree& t) {
        m_root = t.clone();
    }
    BinaryTree& operator=(const BinaryTree& t) {
        m_root = t.clone();
        return *this;
    }
    BinaryTree(BinaryTree&& t) = default;
    BinaryTree& operator=(BinaryTree&& t) = default;
    virtual ~BinaryTree() = default;

    BinaryTree(BinaryTreeNodeInst<T>&& root) : m_root(std::move(root)) {}
    BinaryTree& operator=(BinaryTreeNodeInst<T>&& root) {
        m_root = std::move(root);
        return *this;
    }
    
    BinaryTree(const T& e) {
        m_root = BinaryTreeNodeInst<T>::make(e);
    }

    BinaryTree(T&& e) {
        m_root = BinaryTreeNodeInst<T>::make(std::move(e));
    }

    operator BinaryTreeNodeInst<T>&& () { return std::move(m_root); }

    void clear() override {
        m_root.reset();
    }

    BinaryTreeNodePos<T> insertAsLeftChild(BinaryTreeNodePos<T> p, const T& e) override {
        return attachAsLeftChild(p, BinaryTreeNodeInst<T>::make(e));
    }

    BinaryTreeNodePos<T> insertAsRightChild(BinaryTreeNodePos<T> p, const T& e) override {
        return attachAsRightChild(p, BinaryTreeNodeInst<T>::make(e));
    }

    BinaryTreeNodePos<T> insertAsLeftChild(BinaryTreeNodePos<T> p, T&& e) override {
        return attachAsLeftChild(p, BinaryTreeNodeInst<T>::make(std::move(e)));
    }

    BinaryTreeNodePos<T> insertAsRightChild(BinaryTreeNodePos<T> p, T&& e) override {
        return attachAsRightChild(p, BinaryTreeNodeInst<T>::make(std::move(e)));
    }

    BinaryTreeNodePos<T> insertAsRoot(const T& e) override {
        m_root = BinaryTreeNodeInst<T>::make(e);
        return m_root;
    }

    BinaryTreeNodePos<T> insertAsRoot(T&& e) override {
        m_root = BinaryTreeNodeInst<T>::make(std::move(e));
        return m_root;
    }

    BinaryTreeNodePos<T> attachAsLeftChild(BinaryTreeNodePos<T> p, BinaryTreeNodeInst<T>&& st) override {
        st->parent() = p;
        p->left() = std::move(st);
        return p->left();
    }

    BinaryTreeNodePos<T> attachAsRightChild(BinaryTreeNodePos<T> p, BinaryTreeNodeInst<T>&& st) override {
        st->parent() = p;
        p->right() = std::move(st);
        return p->right();
    }

    BinaryTreeNodePos<T> find(const T& e) const override {
        BinaryTreeNodePos<T> node { nullptr };
        traverseNodes<BinaryTreeInOrderTraverseLinear>([&node, &e](auto p) {
            if (p->data() == e) {
                node = p;
            }
        });
        return node;
    }

    T remove(BinaryTreeNodePos<T> p) override {
        return std::move(detach(p)->data());
    }

    BinaryTreeNodeInst<T> detach(BinaryTreeNodePos<T> p) override {
        if (p == m_root) {
            return std::move(m_root);
        } else {
            if (p->isLeftChild()) {
                return std::move(p->parent()->left());
            } else {
                return std::move(p->parent()->right());
            }
        }
    }

    std::string type_name() const override {
        return "Binary Tree";
    }

    template <template <typename> typename Trav>
        requires std::is_base_of_v<AbstractBinaryTreeTraverse<T>, Trav<T>>
    void traverse(std::function<void(const T&)> visit) const {
        return Trav<T> {} (m_root, visit);
    }

    template <template <typename> typename Trav>
        requires std::is_base_of_v<AbstractBinaryTreeTraverse<T>, Trav<T>>
    void traverseNodes(std::function<void(BinaryTreeNodeConstPos<T>)> visit) const {
        return Trav<T> {}.traverse (m_root, visit);
    }
};

}