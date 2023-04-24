module;
#include <memory>
#include <iterator>

export module BinaryTree.Iterator;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Iterator.AbstractBinaryTreeIterate;
export import BinaryTree.Iterator.BinaryTreePreOrderIterate;
export import BinaryTree.Iterator.BinaryTreeInOrderIterate;
export import BinaryTree.Iterator.BinaryTreePostOrderIterate;

export namespace dslab {

template <typename T, template <typename> typename It>
    requires std::is_base_of_v<AbstractBinaryTreeIterate<T>, It<T>>
class BinaryTreeConstIterator {
protected:
    It<T> m_it;
    BinaryTreeNodeConstPos<T> m_node { nullptr };
public:
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::forward_iterator_tag;

    BinaryTreeConstIterator() = default;
    BinaryTreeConstIterator(BinaryTreeNodeConstPos<T> node) : m_node(node) {}

    static BinaryTreeConstIterator begin(BinaryTreeNodeConstPos<T> node) {
        return BinaryTreeConstIterator { m_it.begin(node) };
    }

    static BinaryTreeConstIterator end(BinaryTreeNodeConstPos<T> node) {
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

template <typename T, template <typename> typename It>
    requires std::is_base_of_v<AbstractBinaryTreeIterate<T>, It<T>>
class BinaryTreeIterator {
protected:
    It<T> m_it;
    BinaryTreeNodePos<T> m_node { nullptr };
public:
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::forward_iterator_tag;

    BinaryTreeIterator() = default;
    BinaryTreeIterator(BinaryTreeNodePos<T> node) : m_node(node) {}

    static BinaryTreeIterator begin(BinaryTreeNodePos<T> node) {
        return BinaryTreeIterator { m_it.begin(node) };
    }

    static BinaryTreeIterator end(BinaryTreeNodePos<T> node) {
        return BinaryTreeIterator { m_it.end(node) };
    }

    BinaryTreeIterator& operator++() {
        m_node = m_it.next(m_node);
        return *this;
    }

    BinaryTreeIterator operator++(int) {
        BinaryTreeIterator tmp(*this);
        m_node = m_it.next(m_node);
        return tmp;
    }

    BinaryTreeIterator& operator+=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            ++(*this);
        return *this;
    }

    BinaryTreeIterator operator+(difference_type n) const {
        BinaryTreeIterator tmp(*this);
        return tmp += n;
    }

    reference operator*() const { return m_node->data(); }
    pointer operator->() const { return &m_node->data(); }

    bool operator==(const BinaryTreeIterator& rhs) const { return m_node == rhs.m_node; }
    auto operator<=>(const BinaryTreeIterator& rhs) const { return m_node <=> rhs.m_node; }
};

}
