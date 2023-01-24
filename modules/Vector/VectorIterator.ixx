module;
#include <type_traits>
#include <iterator>

export module Vector.VectorIterator;

using Rank = size_t;

export namespace dslab {

template <typename T> class AbstractVector;

template <typename Vector>
    requires std::is_base_of_v<AbstractVector<typename Vector::value_type>, Vector>  
class ConstVectorIterator {
protected:
    const Vector* m_vector { nullptr }; // 指向容器的指针
    Rank m_index { 0 }; // 当前元素的秩
    
public:
    using value_type = typename Vector::value_type;
    using difference_type = ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::random_access_iterator_tag;
    
    ConstVectorIterator() = default;
    ConstVectorIterator(const Vector* vector, Rank index) : m_vector(vector), m_index(index) {}
    
    ConstVectorIterator& operator++() { ++m_index; return *this; }
    ConstVectorIterator operator++(int) { auto tmp = *this; ++m_index; return tmp; }
    ConstVectorIterator& operator--() { --m_index; return *this; }
    ConstVectorIterator operator--(int) { auto tmp = *this; --m_index; return tmp; }
    
    ConstVectorIterator& operator+=(difference_type n) { m_index += n; return *this; }
    ConstVectorIterator& operator-=(difference_type n) { m_index -= n; return *this; }
    
    ConstVectorIterator operator+(difference_type n) const { return ConstVectorIterator { m_vector, static_cast<Rank>(m_index + n) }; }
    ConstVectorIterator operator-(difference_type n) const { return ConstVectorIterator { m_vector, static_cast<Rank>(m_index - n) }; }
    
    difference_type operator-(const ConstVectorIterator& rhs) const { return m_index - rhs.m_index; }

    reference operator*() const { return m_vector->operator [](m_index); }
    pointer operator->() const { return &m_vector->operator [](m_index); }
    
    reference operator[](difference_type n) const { return m_vector->operator [](m_index + n); }
    
    bool operator==(const ConstVectorIterator& rhs) const { return m_index == rhs.m_index; }
    auto operator<=>(const ConstVectorIterator& rhs) const { return m_index <=> rhs.m_index; }
};

template <typename Vector>
    requires std::is_base_of_v<AbstractVector<typename Vector::value_type>, Vector>
class VectorIterator {
protected:
    Vector* m_vector { nullptr };
    Rank m_index { 0 };

public:
    using value_type = typename Vector::value_type;
    using difference_type = ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;

    VectorIterator() = default;
    VectorIterator(Vector* vector, Rank index) : m_vector(vector), m_index(index) {}

    VectorIterator& operator++() { ++m_index; return *this; }
    VectorIterator operator++(int) { auto tmp = *this; ++m_index; return tmp; }
    VectorIterator& operator--() { --m_index; return *this; }
    VectorIterator operator--(int) { auto tmp = *this; --m_index; return tmp; }

    VectorIterator& operator+=(difference_type n) { m_index += n; return *this; }
    VectorIterator& operator-=(difference_type n) { m_index -= n; return *this; }
    
    VectorIterator operator+(difference_type n) const { return VectorIterator { m_vector, static_cast<Rank>(m_index + n) }; }
    VectorIterator operator-(difference_type n) const { return VectorIterator { m_vector, static_cast<Rank>(m_index - n) }; }
    
    difference_type operator-(const VectorIterator& rhs) const { return m_index - rhs.m_index; }
    
    reference operator*() const { return m_vector->operator [](m_index); }
    pointer operator->() const { return &m_vector->operator [](m_index); }

    reference operator[](difference_type n) const { return m_vector->operator [](m_index + n); }
    
    bool operator==(const VectorIterator& rhs) const { return m_index == rhs.m_index; }
    auto operator<=>(const VectorIterator& rhs) const { return m_index <=> rhs.m_index; }
};
}