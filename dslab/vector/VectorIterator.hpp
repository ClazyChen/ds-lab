#pragma once

#include "../framework.hpp"

namespace dslab::vector {

    // forward declaration of vector
    template <typename V>
    class VectorIterator;

    // const iterator for vector
    template <typename V>
    class ConstVectorIterator {
    protected:
        // pointer to the vector
        const V* m_vector { nullptr };

        // current rank
        std::size_t m_index { 0 };

        friend class VectorIterator<V>;

    public:
        using value_type = typename V::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::random_access_iterator_tag;

        // default constructor
        constexpr ConstVectorIterator() = default;

        // constructor with vector and rank
        constexpr ConstVectorIterator(const V* vector, std::size_t index) : m_vector(vector), m_index(index) {}

        constexpr ConstVectorIterator& operator++() { ++m_index; return *this; }
        constexpr ConstVectorIterator operator++(int) { auto tmp { *this }; ++m_index; return tmp; }
        constexpr ConstVectorIterator& operator--() { --m_index; return *this; }
        constexpr ConstVectorIterator operator--(int) { auto tmp { *this }; --m_index; return tmp; }

        constexpr ConstVectorIterator& operator+=(difference_type n) { m_index += n; return *this; }
        constexpr ConstVectorIterator& operator-=(difference_type n) { return *this += (-n); }
        
        constexpr friend ConstVectorIterator operator+(difference_type n, const ConstVectorIterator& it) { return ConstVectorIterator { it.m_vector, it.m_index + n }; }
        constexpr ConstVectorIterator operator+(difference_type n) const { return n + *this; }
        constexpr ConstVectorIterator operator-(difference_type n) const { return *this + (-n); }

        constexpr difference_type operator-(const ConstVectorIterator& rhs) const { return m_index - rhs.m_index; }
        constexpr reference operator*() const { return m_vector->operator[](m_index); }
        constexpr pointer operator->() const { return &m_vector->operator[](m_index); }
        constexpr reference operator[](difference_type n) const { return m_vector->operator[](m_index + n); }
        constexpr bool operator==(const ConstVectorIterator& rhs) const { return m_index == rhs.m_index; }
        constexpr auto operator<=>(const ConstVectorIterator& rhs) const { return m_index <=> rhs.m_index; }
    };

    // iterator for vector
    template <typename V>
    class VectorIterator {
    protected:
        // pointer to the vector
        V* m_vector { nullptr };

        // current rank
        std::size_t m_index { 0 };
    
    public:
        using value_type = typename V::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::random_access_iterator_tag;

        // default constructor
        constexpr VectorIterator() = default;

        // constructor with vector and rank
        constexpr VectorIterator(V* vector, std::size_t index) : m_vector(vector), m_index(index) {}

        // conversion from const iterator
        constexpr VectorIterator(const ConstVectorIterator<V>& it) : m_vector(const_cast<V*>(it.m_vector)), m_index(it.m_index) {}

        constexpr VectorIterator& operator++() { ++m_index; return *this; }
        constexpr VectorIterator operator++(int) { auto tmp { *this }; ++m_index; return tmp; }
        constexpr VectorIterator& operator--() { --m_index; return *this; }
        constexpr VectorIterator operator--(int) { auto tmp { *this }; --m_index; return tmp; }

        constexpr VectorIterator& operator+=(difference_type n) { m_index += n; return *this; }
        constexpr VectorIterator& operator-=(difference_type n) { return *this += (-n); }

        constexpr friend VectorIterator operator+(difference_type n, const VectorIterator& it) { return VectorIterator { it.m_vector, it.m_index + n }; }
        constexpr VectorIterator operator+(difference_type n) const { return n + *this; }
        constexpr VectorIterator operator-(difference_type n) const { return *this + (-n); }

        constexpr difference_type operator-(const VectorIterator& rhs) const { return m_index - rhs.m_index; }
        constexpr reference operator*() const { return m_vector->operator[](m_index); }
        constexpr pointer operator->() const { return &m_vector->operator[](m_index); }
        constexpr reference operator[](difference_type n) const { return m_vector->operator[](m_index + n); }
        constexpr bool operator==(const VectorIterator& rhs) const { return m_index == rhs.m_index; }
        constexpr auto operator<=>(const VectorIterator& rhs) const { return m_index <=> rhs.m_index; }

        // conversion to const iterator
        constexpr operator ConstVectorIterator<V>() const { return ConstVectorIterator<V> { m_vector, m_index }; }

    };


}