module;
#include <type_traits>
#include <iterator>

export module ForwardList.ForwardListIterator;

import ForwardList.ForwardListNode;

export namespace dslab {

template <typename T> class AbstractForwardList;

template <typename ForwardList>
    requires std::is_base_of_v<AbstractForwardList<typename ForwardList::value_type>, ForwardList>
class ConstForwardListIterator {
protected:
    const ForwardList* m_list { nullptr };
    ForwardListNodeConstPos<typename ForwardList::value_type> m_node { nullptr };
public:
    using value_type = typename ForwardList::value_type;
    using difference_type = ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::forward_iterator_tag;

    ConstForwardListIterator() = default;
    ConstForwardListIterator(const ForwardList* list, ForwardListNodeConstPos<value_type> node) : m_list(list), m_node(node) {}

    ConstForwardListIterator& operator++() {
        m_node = m_node->next();
        return *this;
    }

    ConstForwardListIterator operator++(int) {
        ConstForwardListIterator tmp(*this);
        m_node = m_node->next();
        return tmp;
    }

    ConstForwardListIterator& operator+=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            ++(*this);
        return *this;
    }

    ConstForwardListIterator operator+(difference_type n) const {
        ConstForwardListIterator tmp(*this);
        return tmp += n;
    }

    reference operator*() const { return m_node->data(); }
    pointer operator->() const { return &m_node->data(); }

    bool operator==(const ConstForwardListIterator& rhs) const { return m_node == rhs.m_node; }
    auto operator<=>(const ConstForwardListIterator& rhs) const { return m_node <=> rhs.m_node; }
};

template <typename ForwardList>
    requires std::is_base_of_v<AbstractForwardList<typename ForwardList::value_type>, ForwardList>
class ForwardListIterator {
protected:
    ForwardList* m_list { nullptr };
    ForwardListNodePos<typename ForwardList::value_type> m_node { nullptr };
public:
    using value_type = typename ForwardList::value_type;
    using difference_type = ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::forward_iterator_tag;

    ForwardListIterator() = default;
    ForwardListIterator(ForwardList* list, ForwardListNodePos<value_type> node) : m_list(list), m_node(node) {}

    ForwardListIterator& operator++() {
        m_node = m_node->next();
        return *this;
    }

    ForwardListIterator operator++(int) {
        ForwardListIterator tmp(*this);
        m_node = m_node->next();
        return tmp;
    }

    ForwardListIterator& operator+=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            ++(*this);
        return *this;
    }

    ForwardListIterator operator+(difference_type n) const {
        ForwardListIterator tmp(*this);
        return tmp += n;
    }

    reference operator*() const { return m_node->data(); }
    pointer operator->() const { return &m_node->data(); }

    bool operator==(const ForwardListIterator& rhs) const { return m_node == rhs.m_node; }
    auto operator<=>(const ForwardListIterator& rhs) const { return m_node <=> rhs.m_node; }
};

}