#pragma once

#include "LinearList.hpp"
#include "VectorIterator.hpp"

namespace dslab::vector {

template <typename T>
class AbstractVector : public LinearList<T, VectorIterator<AbstractVector<T>>, ConstVectorIterator<AbstractVector<T>>> {
protected:
    // return the first address of the vector
    virtual T* data() = 0;
    virtual const T* data() const = 0;
public:
    using value_type = T;
    using iterator = VectorIterator<AbstractVector<T>>;
    using const_iterator = ConstVectorIterator<AbstractVector<T>>;

    // return the capacity of the vector
    virtual std::size_t capacity() const = 0;

    // reserve the capacity of the vector
    virtual void reserve(std::size_t n) = 0;

    // return the size of the vector (has been declared in DataStructure)
    virtual std::size_t size() const = 0;

    // resize the vector
    virtual void resize(std::size_t n) = 0;

    // override operator[] to get the element at index r
    // TODO: deducing this in C++23 to avoid const overloading
    T& operator[](std::size_t r) {
        return data()[r];
    }
    const T& operator[](std::size_t r) const {
        return data()[r];
    }

    // override the methods in LinearList
    void clear() override {
        resize(0);
    }

    // iterator-related methods below

    iterator begin() noexcept override {
        return iterator { this, 0 };
    }

    iterator end() noexcept override {
        return iterator { this, size() };
    }

    const_iterator begin() const noexcept {
        return const_iterator { this, 0 };
    }

    const_iterator end() const noexcept {
        return const_iterator { this, size() };
    }

    const_iterator cbegin() const noexcept override {
        return const_iterator { this, 0 };
    }

    const_iterator cend() const noexcept override {
        return const_iterator { this, size() };
    }

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    reverse_iterator rbegin() noexcept {
        return reverse_iterator { end() };
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator { begin() };
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator { end() };
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator { begin() };
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator { cend() };
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator { cbegin() };
    }

};

}