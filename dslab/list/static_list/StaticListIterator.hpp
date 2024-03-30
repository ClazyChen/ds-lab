#pragma once

#include "../../framework.hpp"

namespace dslab::list::static_list {

    template <typename L>
    class StaticListIterator;

    // const iterator for static list
    template <typename L>
    class ConstStaticListIterator {
    protected:
        // list to iterate
        const L* m_list { nullptr };

        // current node
        std::size_t m_index { 0 };

        friend class StaticListIterator<L>;
    public:
        using value_type = typename L::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::bidirectional_iterator_tag;

        constexpr ConstStaticListIterator() = default;
        constexpr ConstStaticListIterator(const L* list, std::size_t index) : m_list(list), m_index(index) {}

        constexpr ConstStaticListIterator& operator++() {
            m_index = m_list->node(m_index).next();
            return *this;
        }

        constexpr ConstStaticListIterator operator++(int) {
            ConstStaticListIterator tmp(*this);
            m_index = m_list->node(m_index).next();
            return tmp;
        }

        constexpr ConstStaticListIterator& operator--() {
            m_index = m_list->node(m_index).prev();
            return *this;
        }

        constexpr ConstStaticListIterator operator--(int) {
            ConstStaticListIterator tmp(*this);
            m_index = m_list->node(m_index).prev();
            return tmp;
        }

        constexpr ConstStaticListIterator& operator+=(difference_type n) {
            for (difference_type i { 0 }; i < n; ++i)
                ++(*this);
            return *this;
        }

        constexpr ConstStaticListIterator& operator-=(difference_type n) {
            for (difference_type i { 0 }; i < n; ++i)
                --(*this);
            return *this;
        }

        constexpr friend ConstStaticListIterator operator+(difference_type n, const ConstStaticListIterator& it) {
            return it + n;
        }
        constexpr ConstStaticListIterator operator+(difference_type n) const {
            ConstStaticListIterator tmp(*this);
            return tmp += n;
        }
        constexpr ConstStaticListIterator operator-(difference_type n) const {
            ConstStaticListIterator tmp(*this);
            return tmp -= n;
        }

        constexpr const typename L::node_type& node() const { return m_list->node(m_index); }
        constexpr std::size_t index() const { return m_index; }

        constexpr difference_type operator-(const ConstStaticListIterator& rhs) const {
            difference_type diff { 0 };
            for (auto i { *this }; i != rhs; ++i)
                ++diff;
            return diff;
        }

        constexpr reference operator*() const { return m_list->node(m_index).data(); }
        constexpr pointer operator->() const { return &m_list->node(m_index).data(); }

        constexpr bool operator==(const ConstStaticListIterator& rhs) const { return m_index == rhs.m_index; }
        constexpr auto operator<=>(const ConstStaticListIterator& rhs) const { return m_index <=> rhs.m_index; }
    };

    // iterator for static list
    template <typename L>
    class StaticListIterator {
    protected:
        // list to iterate
        L* m_list { nullptr };

        // current node
        std::size_t m_index { 0 };
    public:
        using value_type = typename L::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::bidirectional_iterator_tag;

        constexpr StaticListIterator() = default;
        constexpr StaticListIterator(L* list, std::size_t index) : m_list(list), m_index(index) {}

        // convert from const iterator
        constexpr StaticListIterator(const ConstStaticListIterator<L>& it) : m_list(const_cast<L*>(it.m_list)), m_index(it.m_index) {}

        constexpr StaticListIterator& operator++() {
            m_index = m_list->node(m_index).next();
            return *this;
        }

        constexpr StaticListIterator operator++(int) {
            StaticListIterator tmp(*this);
            m_index = m_list->node(m_index).next();
            return tmp;
        }

        constexpr StaticListIterator& operator--() {
            m_index = m_list->node(m_index).prev();
            return *this;
        }

        constexpr StaticListIterator operator--(int) {
            StaticListIterator tmp(*this);
            m_index = m_list->node(m_index).prev();
            return tmp;
        }

        constexpr StaticListIterator& operator+=(difference_type n) {
            for (difference_type i { 0 }; i < n; ++i)
                ++(*this);
            return *this;
        }

        constexpr StaticListIterator& operator-=(difference_type n) {
            for (difference_type i { 0 }; i < n; ++i)
                --(*this);
            return *this;
        }

        constexpr friend StaticListIterator operator+(difference_type n, const StaticListIterator& it) {
            return it + n;
        }
        constexpr StaticListIterator operator+(difference_type n) const {
            StaticListIterator tmp(*this);
            return tmp += n;
        }
        constexpr StaticListIterator operator-(difference_type n) const {
            StaticListIterator tmp(*this);
            return tmp -= n;
        }

        constexpr typename L::node_type& node() const { return m_list->node(m_index); }
        constexpr std::size_t index() const { return m_index; }

        constexpr difference_type operator-(const StaticListIterator& rhs) const {
            difference_type diff { 0 };
            for (auto i { *this }; i != rhs; ++i)
                ++diff;
            return diff;
        }

        constexpr reference operator*() const { return m_list->node(m_index).data(); }
        constexpr pointer operator->() const { return &m_list->node(m_index).data(); }

        constexpr bool operator==(const StaticListIterator& rhs) const { return m_index == rhs.m_index; }
        constexpr auto operator<=>(const StaticListIterator& rhs) const { return m_index <=> rhs.m_index; }

        constexpr bool operator==(const ConstStaticListIterator<L>& rhs) const { return m_index == rhs.m_index; }
        constexpr auto operator<=>(const ConstStaticListIterator<L>& rhs) const { return m_index <=> rhs.m_index; }

        constexpr friend bool operator==(const ConstStaticListIterator<L>& lhs, const StaticListIterator& rhs) { return lhs.m_index == rhs.m_index; }
        constexpr friend auto operator<=>(const ConstStaticListIterator<L>& lhs, const StaticListIterator& rhs) { return lhs.m_index <=> rhs.m_index; }

        // convert to const iterator
        constexpr operator ConstStaticListIterator<L>() const { return ConstStaticListIterator<L>(m_list, m_index); }
    };

}