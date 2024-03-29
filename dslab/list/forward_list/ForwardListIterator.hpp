#pragma once

#include "../../framework.hpp"

namespace dslab::list::forward_list {

    template <typename L>
    class ForwardListIterator;

    // const iterator for list
    template <typename L>
    class ConstForwardListIterator {
    protected:
        // list to iterate
        const L* m_list { nullptr };
        
        // current node
        const typename L::node_type* m_node { nullptr };

        friend class ForwardListIterator<L>;

    public:
        using value_type = typename L::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::forward_iterator_tag;

        constexpr ConstForwardListIterator() = default;
        constexpr ConstForwardListIterator(const L* list, const typename L::node_type* node) : m_list(list), m_node(node) {}

        constexpr ConstForwardListIterator& operator++() {
            m_node = m_node->next().get();
            return *this;
        }
        constexpr ConstForwardListIterator operator++(int) {
            ConstForwardListIterator tmp(*this);
            m_node = m_node->next().get();
            return tmp;
        }
        // --node 有定义，但永远不会被调用
        [[maybe_unused]] constexpr ConstForwardListIterator& operator--() { return *this; }
        [[maybe_unused]] constexpr ConstForwardListIterator operator--(int) { return *this; }

        constexpr ConstForwardListIterator& operator+=(difference_type n) {
            for (difference_type i { 0 }; i < n; ++i)
                ++(*this);
            return *this;
        }
        
        constexpr friend ConstForwardListIterator operator+(difference_type n, const ConstForwardListIterator& it) {
            return it + n;
        }
        constexpr ConstForwardListIterator operator+(difference_type n) const {
            ConstForwardListIterator tmp(*this);
            return tmp += n;
        }

        constexpr const typename L::node_type* node() const {
            return m_node;
        }

        constexpr reference operator*() const {
            return m_node->data();
        }

        constexpr pointer operator->() const {
            return &m_node->data();
        }

        constexpr bool operator==(const ConstForwardListIterator& other) const {
            return m_node == other.m_node;
        }
        constexpr auto operator<=>(const ConstForwardListIterator& other) const {
            return m_node <=> other.m_node;
        }
    };

    // iterator for list
    template <typename L>
    class ForwardListIterator {
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
        using iterator_category = std::forward_iterator_tag;

        constexpr ForwardListIterator() = default;
        constexpr ForwardListIterator(L* list, typename L::node_type* node) : m_list(list), m_node(node) {}

        // convert from const iterator
        constexpr ForwardListIterator(const ConstForwardListIterator<L>& it) : m_list(const_cast<L*>(it.m_list)), m_node(const_cast<typename L::node_type*>(it.m_node)) {}

        constexpr ForwardListIterator& operator++() {
            m_node = m_node->next().get();
            return *this;
        }
        constexpr ForwardListIterator operator++(int) {
            ForwardListIterator tmp(*this);
            m_node = m_node->next().get();
            return tmp;
        }

        constexpr ForwardListIterator& operator+=(difference_type n) {
            for (difference_type i { 0 }; i < n; ++i)
                ++(*this);
            return *this;
        }

        constexpr friend ForwardListIterator operator+(difference_type n, const ForwardListIterator& it) {
            return it + n;
        }
        constexpr ForwardListIterator operator+(difference_type n) const {
            ForwardListIterator tmp(*this);
            return tmp += n;
        }

        constexpr typename L::node_type* node() const {
            return m_node;
        }

        constexpr reference operator*() const {
            return m_node->data();
        }

        constexpr pointer operator->() const {
            return &m_node->data();
        }

        constexpr bool operator==(const ForwardListIterator& other) const {
            return m_node == other.m_node;
        }
        constexpr auto operator<=>(const ForwardListIterator& other) const {
            return m_node <=> other.m_node;
        }
        constexpr bool operator==(const ConstForwardListIterator<L>& other) const {
            return m_node == other.node();
        }
        constexpr auto operator<=>(const ConstForwardListIterator<L>& other) const {
            return m_node <=> other.node();
        }
        constexpr friend bool operator==(const ConstForwardListIterator<L>& lhs, const ForwardListIterator& rhs) {
            return lhs.node() == rhs.m_node;
        }
        constexpr friend auto operator<=>(const ConstForwardListIterator<L>& lhs, const ForwardListIterator& rhs) {
            return lhs.node() <=> rhs.m_node;
        }

        constexpr operator ConstForwardListIterator<L>() const {
            return ConstForwardListIterator<L> { m_list, m_node };
        }
    };
}