#pragma once

#include "../../framework.hpp"

namespace dslab::list::bidirectional_list {

    template <typename L>
    class ListIterator;

    // const iterator for list
    template <typename L>
    class ConstListIterator {
    protected:
        // list to iterate
        const L* m_list { nullptr };
        
        // current node
        const typename L::node_type* m_node { nullptr };

        friend class ListIterator<L>;
    
    public:
        using value_type = typename L::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::bidirectional_iterator_tag;

        constexpr ConstListIterator() = default;
        constexpr ConstListIterator(const L* list, const typename L::node_type* node) : m_list(list), m_node(node) {}

        constexpr ConstListIterator& operator++() {
            m_node = m_node->next().get();
            return *this;
        }
        constexpr ConstListIterator operator++(int) {
            ConstListIterator tmp(*this);
            m_node = m_node->next().get();
            return tmp;
        }
        constexpr ConstListIterator& operator--() {
            m_node = m_node->prev();
            return *this;
        }
        constexpr ConstListIterator operator--(int) {
            ConstListIterator tmp(*this);
            m_node = m_node->prev();
            return tmp;
        }

        constexpr ConstListIterator& operator+=(difference_type n) {
            for (difference_type i { 0 }; i < n; ++i)
                ++(*this);
            return *this;
        }
        constexpr ConstListIterator& operator-=(difference_type n) {
            for (difference_type i { 0 }; i < n; ++i)
                --(*this);
            return *this;
        }
        
        constexpr friend ConstListIterator operator+(difference_type n, const ConstListIterator& it) {
            return it + n;
        }
        constexpr ConstListIterator operator+(difference_type n) const {
            ConstListIterator tmp(*this);
            return tmp += n;
        }
        constexpr ConstListIterator operator-(difference_type n) const {
            ConstListIterator tmp(*this);
            return tmp -= n;
        }

        constexpr const typename L::node_type* node() const { return m_node; }

        constexpr difference_type operator-(const ConstListIterator& rhs) const {
            difference_type count { 0 };
            for (auto it { *this }; it != rhs; ++it)
                ++count;
            return count;
        }

        constexpr reference operator*() const { return m_node->data(); }
        constexpr pointer operator->() const { return &m_node->data(); }

        constexpr bool operator==(const ConstListIterator& rhs) const { return m_node == rhs.m_node; }
        constexpr auto operator<=>(const ConstListIterator& rhs) const { return m_node <=> rhs.m_node; }

    };

    // iterator for list

    template <typename L>
    class ListIterator {
    protected:
        // list to iterate
        L* m_list { nullptr };
        
        // current node
        typename L::node_type* m_node { nullptr };
    
    public:
        using value_type = typename L::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::bidirectional_iterator_tag;

        constexpr ListIterator() = default;
        constexpr ListIterator(L* list, typename L::node_type* node) : m_list(list), m_node(node) {}

        // convert from const iterator
        constexpr ListIterator(const ConstListIterator<L>& it) : m_list(const_cast<L*>(it.m_list)), m_node(const_cast<typename L::node_type*>(it.m_node)) {}

        constexpr ListIterator& operator++() {
            m_node = m_node->next().get();
            return *this;
        }
        constexpr ListIterator operator++(int) {
            ListIterator tmp(*this);
            m_node = m_node->next().get();
            return tmp;
        }

        constexpr ListIterator& operator--() {
            m_node = m_node->prev();
            return *this;
        }
        constexpr ListIterator operator--(int) {
            ListIterator tmp(*this);
            m_node = m_node->prev();
            return tmp;
        }

        constexpr ListIterator& operator+=(difference_type n) {
            for (difference_type i { 0 }; i < n; ++i)
                ++(*this);
            return *this;
        }
        constexpr ListIterator& operator-=(difference_type n) {
            for (difference_type i { 0 }; i < n; ++i)
                --(*this);
            return *this;
        }

        constexpr friend ListIterator operator+(difference_type n, const ListIterator& it) {
            return it + n;
        }
        constexpr ListIterator operator+(difference_type n) const {
            ListIterator tmp(*this);
            return tmp += n;
        }
        constexpr ListIterator operator-(difference_type n) const {
            ListIterator tmp(*this);
            return tmp -= n;
        }

        constexpr typename L::node_type* node() const { return m_node; }

        constexpr difference_type operator-(const ListIterator& rhs) const {
            difference_type count { 0 };
            for (auto it { *this }; it != rhs; ++it)
                ++count;
            return count;
        }

        constexpr reference operator*() const { return m_node->data(); }
        constexpr pointer operator->() const { return &m_node->data(); }

        constexpr bool operator==(const ListIterator& rhs) const { return m_node == rhs.m_node; }
        constexpr auto operator<=>(const ListIterator& rhs) const { return m_node <=> rhs.m_node; }

        constexpr bool operator==(const ConstListIterator<L>& rhs) const { return m_node == rhs.node(); }
        constexpr auto operator<=>(const ConstListIterator<L>& rhs) const { return m_node <=> rhs.node(); }

        constexpr friend bool operator==(const ConstListIterator<L>& lhs, const ListIterator& rhs) { return lhs.node() == rhs.m_node; }
        constexpr friend auto operator<=>(const ConstListIterator<L>& lhs, const ListIterator& rhs) { return lhs.node() <=> rhs.m_node; }
        
        // convert to const iterator
        constexpr operator ConstListIterator<L>() const { return ConstListIterator<L>(m_list, m_node); }

    };

}