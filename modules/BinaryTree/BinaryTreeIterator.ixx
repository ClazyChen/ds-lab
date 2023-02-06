module;
#include <memory>
#include <iterator>

export module BinaryTree.BinaryTreeIterator;

import BinaryTree.BinaryTreeNode;
import Framework.Algorithm;

export namespace dslab {

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>
class BinaryTreeIterate : public Algorithm<Node<T>*, Node<T>*> {
public:
    virtual Node<T>* begin(Node<T>* node) = 0;
    virtual Node<T>* end(Node<T>* node) {
        return nullptr;
    }
};

template <typename T, template <typename, template <typename> typename> typename It, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<BinaryTreeNode<T>, Node<T>>&& std::is_base_of_v<BinaryTreeIterate<T, Node>, It<T, Node>>
class BinaryTreeConstIterator {
protected:
    It<T, Node> it;
    Node<T>* node { nullptr };
public:
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::forward_iterator_tag;

    BinaryTreeConstIterator() = default;
    BinaryTreeConstIterator(Node<T>* node) : node(node) {}

    static BinaryTreeConstIterator begin(Node<T>* node) {
        return BinaryTreeConstIterator(it.begin(node));
    }

    static BinaryTreeConstIterator end(Node<T>* node) {
        return BinaryTreeConstIterator(it.end(node));
    }

    BinaryTreeConstIterator& operator++() {
        node = it(node);
        return *this;
    }

    BinaryTreeConstIterator operator++(int) {
        BinaryTreeConstIterator tmp(*this);
        node = it(node);
        return tmp;
    }

    BinaryTreeConstIterator& operator+=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            ++(*this);
        return *this;
    }

    BinaryTreeConstIterator operator+(difference_type n) const {
        BinaryTreeConstIterator tmp(*this);
        return tmp += n;
    }

    reference operator*() const { return node->data(); }
    pointer operator->() const { return &node->data(); }

    bool operator==(const BinaryTreeConstIterator& rhs) const { return node == rhs.node; }
    auto operator<=>(const BinaryTreeConstIterator& rhs) const { return node <=> rhs.node; }
};

}
