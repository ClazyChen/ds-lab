module;
#include <type_traits>
#include <iterator>

export module List.ListIterator;

import List.ListNode;

export namespace dslab {

template <typename T> class AbstractList;
template <typename T> class AbstractCircularList;

template <typename List>
    requires std::is_base_of_v<AbstractList<typename List::value_type>, List>
class ConstListIterator {
protected:
    const List* m_list { nullptr };
    ListNode<typename List::value_type>* m_node { nullptr };
public:
    using value_type = typename List::value_type;
    using difference_type = ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::bidirectional_iterator_tag;

    ConstListIterator() = default;
    ConstListIterator(const List* list, ListNode<value_type>* node) : m_list(list), m_node(node) {}

    ConstListIterator& operator++() {
        m_node = m_node->next();
        return *this;
    }
    ConstListIterator operator++(int) {
        ConstListIterator tmp(*this);
        m_node = m_node->next();
        return tmp;
    }
    ConstListIterator& operator--() {
        m_node = m_node->prev();
        return *this;
    }
    ConstListIterator operator--(int) {
        ConstListIterator tmp(*this);
        m_node = m_node->prev();
        return tmp;
    }

    ConstListIterator& operator+=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            ++(*this);
        return *this;
    }
    ConstListIterator& operator-=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            --(*this);
        return *this;
    }
    ConstListIterator operator+(difference_type n) const {
        ConstListIterator tmp(*this);
        return tmp += n;
    }
    ConstListIterator operator-(difference_type n) const {
        ConstListIterator tmp(*this);
        return tmp -= n;
    }

    reference operator*() const { return m_node->data(); }
    pointer operator->() const { return &m_node->data(); }

    bool operator==(const ConstListIterator& rhs) const { return m_node == rhs.m_node; }
    auto operator<=>(const ConstListIterator& rhs) const { return m_node <=> rhs.m_node; }
};

template <typename List>
    requires std::is_base_of_v<AbstractList<typename List::value_type>, List>
class ListIterator {
protected:
    List* m_list { nullptr };
    ListNode<typename List::value_type>* m_node { nullptr };
public:
    using value_type = typename List::value_type;
    using difference_type = ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::bidirectional_iterator_tag;

    ListIterator() = default;
    ListIterator(List* list, ListNode<value_type>* node) : m_list(list), m_node(node) {}

    ListIterator& operator++() {
        m_node = m_node->next();
        return *this;
    }
    ListIterator operator++(int) {
        ListIterator tmp(*this);
        m_node = m_node->next();
        return tmp;
    }
    ListIterator& operator--() {
        m_node = m_node->prev();
        return *this;
    }
    ListIterator operator--(int) {
        ListIterator tmp(*this);
        m_node = m_node->prev();
        return tmp;
    }

    ListIterator& operator+=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            ++(*this);
        return *this;
    }
    ListIterator& operator-=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            --(*this);
        return *this;
    }
    ListIterator operator+(difference_type n) const {
        ListIterator tmp(*this);
        return tmp += n;
    }
    ListIterator operator-(difference_type n) const {
        ListIterator tmp(*this);
        return tmp -= n;
    }

    reference operator*() const { return m_node->data(); }
    pointer operator->() const { return &m_node->data(); }

    bool operator==(const ListIterator& rhs) const { return m_node == rhs.m_node; }
    auto operator<=>(const ListIterator& rhs) const { return m_node <=> rhs.m_node; }
};

}