module;
#include <memory>
#include <iterator>

export module BinaryTree.BinaryTreeIterator;

import BinaryTree.AbstractBinaryTreeNode;

export namespace dslab {

template <typename T>
class BinaryTreeIterate {
public:
    virtual const AbstractBinaryTreeNode<T>* begin(const AbstractBinaryTreeNode<T>* node) = 0;
    virtual const AbstractBinaryTreeNode<T>* end(const AbstractBinaryTreeNode<T>* node) {
        return nullptr;
    }
    virtual const AbstractBinaryTreeNode<T>* next(const AbstractBinaryTreeNode<T>* node) = 0;
};

template <typename T, template <typename> typename It>
    requires std::is_base_of_v<BinaryTreeIterate<T>, It<T>>
class BinaryTreeConstIterator {
protected:
    It<T> m_it;
    const AbstractBinaryTreeNode<T>* m_node { nullptr };
public:
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::forward_iterator_tag;

    BinaryTreeConstIterator() = default;
    BinaryTreeConstIterator(const AbstractBinaryTreeNode<T>* node) : m_node(node) {}

    static BinaryTreeConstIterator begin(const AbstractBinaryTreeNode<T>* node) {
        return BinaryTreeConstIterator { m_it.begin(node) };
    }

    static BinaryTreeConstIterator end(const AbstractBinaryTreeNode<T>* node) {
        return BinaryTreeConstIterator { m_it.end(node) };
    }

    BinaryTreeConstIterator& operator++() {
        m_node = m_it.next(m_node);
        return *this;
    }

    BinaryTreeConstIterator operator++(int) {
        BinaryTreeConstIterator tmp(*this);
        m_node = m_it.next(m_node);
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

    reference operator*() const { return m_node->data(); }
    pointer operator->() const { return &m_node->data(); }

    bool operator==(const BinaryTreeConstIterator& rhs) const { return m_node == rhs.m_node; }
    auto operator<=>(const BinaryTreeConstIterator& rhs) const { return m_node <=> rhs.m_node; }
};

}
