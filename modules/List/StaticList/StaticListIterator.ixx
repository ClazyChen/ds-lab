module;
#include <type_traits>
#include <iterator>

export module List.StaticList.StaticListIterator;

import List.StaticList.StaticListNode;

using Rank = size_t;

export namespace dslab {

template <typename T> class AbstractStaticList;

template <typename StaticList>
    requires std::is_base_of_v<AbstractStaticList<typename StaticList::value_type>, StaticList>
class ConstStaticListIterator {
protected:
    const StaticList* m_list { nullptr };
    Rank m_index { 0 };
public:
    using value_type = typename StaticList::value_type;
    using difference_type = ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::bidirectional_iterator_tag;

    ConstStaticListIterator() = default;
    ConstStaticListIterator(const StaticList* list, Rank index) : m_list(list), m_index(index) {}

    ConstStaticListIterator& operator++() {
        m_index = m_list->next(m_index);
        return *this;
    }

    ConstStaticListIterator operator++(int) {
        ConstStaticListIterator tmp(*this);
        m_index = m_list->next(m_index);
        return tmp;
    }

    ConstStaticListIterator& operator--() {
        m_index = m_list->prev(m_index);
        return *this;
    }
    
    ConstStaticListIterator operator--(int) {
        ConstStaticListIterator tmp(*this);
        m_index = m_list->prev(m_index);
        return tmp;
    }

    ConstStaticListIterator& operator+=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            ++(*this);
        return *this;
    }
    
    ConstStaticListIterator& operator-=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            --(*this);
        return *this;
    }

    ConstStaticListIterator operator+(difference_type n) const {
        ConstStaticListIterator tmp(*this);
        return tmp += n;
    }

    ConstStaticListIterator operator-(difference_type n) const {
        ConstStaticListIterator tmp(*this);
        return tmp -= n;
    }

    reference operator*() const {
        return m_list->get(m_index);
    }
    
    pointer operator->() const {
        return &m_list->get(m_index);
    }

    bool operator==(const ConstStaticListIterator& other) const {
        return m_index == other.m_index;
    }

    auto operator<=>(const ConstStaticListIterator& other) const {
        return m_index <=> other.m_index;
    }

};

template <typename StaticList>
    requires std::is_base_of_v<AbstractStaticList<typename StaticList::value_type>, StaticList>
class StaticListIterator {
protected:
    StaticList* m_list { nullptr };
    Rank m_index { 0 };
public:
    using value_type = typename StaticList::value_type;
    using difference_type = ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::bidirectional_iterator_tag;

    StaticListIterator() = default;
    StaticListIterator(StaticList* list, Rank index) : m_list(list), m_index(index) {}
    
    StaticListIterator& operator++() {
        m_index = m_list->next(m_index);
        return *this;
    }
    
    StaticListIterator operator++(int) {
        StaticListIterator tmp(*this);
        m_index = m_list->next(m_index);
        return tmp;
    }

    StaticListIterator& operator--() {
        m_index = m_list->prev(m_index);
        return *this;
    }

    StaticListIterator operator--(int) {
        StaticListIterator tmp(*this);
        m_index = m_list->prev(m_index);
        return tmp;
    }

    StaticListIterator& operator+=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            ++(*this);
        return *this;
    }

    StaticListIterator& operator-=(difference_type n) {
        for (difference_type i = 0; i < n; ++i)
            --(*this);
        return *this;
    }

    StaticListIterator operator+(difference_type n) const {
        StaticListIterator tmp(*this);
        return tmp += n;
    }

    StaticListIterator operator-(difference_type n) const {
        StaticListIterator tmp(*this);
        return tmp -= n;
    }

    reference operator*() const {
        return m_list->get(m_index);
    }

    pointer operator->() const {
        return &m_list->get(m_index);
    }

    bool operator==(const StaticListIterator& other) const {
        return m_index == other.m_index;
    }
    
    auto operator<=>(const StaticListIterator& other) const {
        return m_index <=> other.m_index;
    }
};

}