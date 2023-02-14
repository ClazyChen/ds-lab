module;
#include <memory>

export module BinaryTree;

export import BinaryTree.BinaryTreeNode;
export import BinaryTree.AbstractBinaryTreeNode;
export import BinaryTree.AbstractBinaryTree;
export import BinaryTree.BinaryTreeTraverse;
export import BinaryTree.BinaryTreeInorderTraverse;
export import BinaryTree.BinaryTreePostorderTraverse;
export import BinaryTree.BinaryTreePreorderTraverse;

export namespace dslab {

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<AbstractBinaryTreeNode<T>, Node<T>>
class BinaryTree : public AbstractBinaryTree<T, Node> {
    std::unique_ptr<AbstractBinaryTreeNode<T>> m_root { nullptr };
    size_t m_size { 0 };

    std::unique_ptr<AbstractBinaryTreeNode<T>> clone(const Node<T>* node) const {
        std::unique_ptr<AbstractBinaryTreeNode<T>> newNode { std::make_unique<Node<T>>(node->data()) };
        if (node->left() != nullptr) {
            newNode->left() = clone(node->left().get());
        }
        if (node->right() != nullptr) {
            newNode->right() = clone(node->right().get());
        }
        return newNode;
    }

    Node<T>* get(std::unique_ptr<AbstractBinaryTreeNode<T>>& node) {
        return static_cast<Node<T>*>(node.get());
    }
    Node<T>* get(AbstractBinaryTreeNode<T>* node) {
        return static_cast<Node<T>*>(node);
    }
    const Node<T>* get(const std::unique_ptr<AbstractBinaryTreeNode<T>>& node) const {
        return static_cast<const Node<T>*>(node.get());
    }
    const Node<T>* get(const AbstractBinaryTreeNode<T>* node) const {
        return static_cast<const Node<T>*>(node);
    }

public:
    Node<T>* root() override { return get(m_root); }
    const Node<T>* root() const {
        return const_cast<BinaryTree*>(this)->root();
    }
    size_t size() const override { return m_size; }

    BinaryTree() = default;
    BinaryTree(const BinaryTree& tree) : m_root { clone(tree.m_root.get()) }, m_size { tree.m_size } {}
    BinaryTree(BinaryTree&& tree) : m_root { std::move(tree.m_root) }, m_size { tree.m_size } {}
    BinaryTree& operator=(const BinaryTree& tree) {
        if (this != &tree) {
            BinaryTree tmp { tree };
            std::swap(m_root, tmp.m_root);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    BinaryTree& operator=(BinaryTree&& tree) {
        if (this != &tree) {
            BinaryTree tmp { std::move(tree) };
            std::swap(m_root, tmp.m_root);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    BinaryTree(std::unique_ptr<AbstractBinaryTreeNode<T>> root) : m_root { std::move(root) } {
        m_size = size(root());
    }

    size_t size(const Node<T>* node) const {
        if (node == nullptr) {
            return 0;
        }
        return 1 + size(get(node->left())) + size(get(node->right()));
    }

    Node<T>* insertAsLeftChild(Node<T>* node, const T& e) override {
        std::unique_ptr<AbstractBinaryTreeNode<T>> newNode { std::make_unique<Node<T>>(e) };
        newNode->parent() = node;
        m_size -= size(get(node->left()));
        node->left() = std::move(newNode);
        ++m_size;
        return get(node->left());
    }

    Node<T>* insertAsRightChild(Node<T>* node, const T& e) override {
        std::unique_ptr<AbstractBinaryTreeNode<T>> newNode { std::make_unique<Node<T>>(e) };
        newNode->parent() = node;
        m_size -= size(get(node->right()));
        node->right() = std::move(newNode);
        ++m_size;
        return get(node->right());
    }

    Node<T>* insertAsLeftChild(Node<T>* node, T&& e) override {
        std::unique_ptr<AbstractBinaryTreeNode<T>> newNode { std::make_unique<Node<T>>(std::move(e)) };
        newNode->parent() = node;
        m_size -= size(get(node->left()));
        node->left() = std::move(newNode);
        ++m_size;
        return get(node->left());
    }

    Node<T>* insertAsRightChild(Node<T>* node, T&& e) override {
        std::unique_ptr<AbstractBinaryTreeNode<T>> newNode { std::make_unique<Node<T>>(std::move(e)) };
        newNode->parent() = node;
        m_size -= size(get(node->right()));
        node->right() = std::move(newNode);
        ++m_size;
        return get(node->right());
    }

    Node<T>* insertAsRoot(const T& e) override {
        m_root = std::make_unique<Node<T>>(e);
        m_size = 1;
        return root();
    }

    Node<T>* insertAsRoot(T&& e) override {
        m_root = std::make_unique<Node<T>>(std::move(e));
        m_size = 1;
        return root();
    }

    Node<T>* attachAsLeftChild(Node<T>* node, AbstractBinaryTree<T, Node>* tree) override {
        m_size -= size(get(node->left()));
        m_size += tree->size();
        node->left() = std::move(tree->detach(tree->root()));
        node->left()->parent() = node;
        return get(node->left());
    }

    Node<T>* attachAsRightChild(Node<T>* node, AbstractBinaryTree<T, Node>* tree) override {
        m_size -= size(get(node->right()));
        m_size += tree->size();
        node->right() = std::move(tree->detach(tree->root()));
        node->right()->parent() = node;
        return get(node->right());
    }

    Node<T>* find(const T& e) override {
        auto node { root() };
        while (node != nullptr) {
            if (node->data() == e) {
                return node;
            }
            if (node->data() < e) {
                node = get(node->right());
            } else {
                node = get(node->left());
            }
        }
        return nullptr;
    }

    const Node<T>* find(const T& e) const {
        return const_cast<BinaryTree*>(this)->find(e);
    }

    T remove(Node<T>* node) override {
        auto data { std::move(node->data()) };
        m_size -= size(node);
        if (auto parent { node->parent() }; parent == nullptr) {
            m_root = nullptr;
        } else if (get(parent->left()) == node) {
            parent->left() = nullptr;
        } else {
            parent->right() = nullptr;
        }
        return data;
    }

    std::unique_ptr<AbstractBinaryTreeNode<T>> detach(Node<T>* node) override {
        std::unique_ptr<AbstractBinaryTreeNode<T>> detached { nullptr };
        m_size -= size(node);
        if (auto parent { node->parent() }; parent == nullptr) {
            detached = std::move(m_root);
        } else if (get(parent->left()) == node) {
            detached = std::move(parent->left());
        } else {
            detached = std::move(parent->right());
        }
        return detached;
    }

    void clear() override {
        m_root = nullptr;
        m_size = 0;
    }
};

}