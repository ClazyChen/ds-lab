#pragma once

#include "AbstractVector.hpp"
#include "VectorAllocator.hpp"

namespace dslab::vector {

template <typename T, typename A = VectorAllocatorGP<std::ratio<3, 2>>>
    requires std::is_base_of_v<VectorAllocator, A>
class Vector : public AbstractVector<T> {
protected:
    std::unique_ptr<T[]> m_data { nullptr };
    std::size_t m_capacity { 0 };
    std::size_t m_size { 0 };
    A m_allocator {};

    T* data() override { return m_data.get(); }
    const T* data() const override { return m_data.get(); }

public:
    std::size_t capacity() const override { return m_capacity; }
    std::size_t size() const override { return m_size; }

    Vector() = default;
    Vector(std::size_t n) : m_data { std::make_unique<T[]>(n) }, m_capacity { n }, m_size { n } {}
    Vector(const Vector& other) : Vector(other.m_size) { std::copy_n(other.m_data.get(), other.m_size, m_data.get()); }
    Vector(Vector&& other) noexcept : m_data { std::move(other.m_data) }, m_capacity { other.m_capacity }, m_size { other.m_size } {
        other.m_capacity = 0;
        other.m_size = 0;
    }
    Vector(std::initializer_list<T> ilist) : Vector(ilist.size()) {
        std::move(ilist.begin(), ilist.end(), m_data.get());
    }

    virtual ~Vector() = default;

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector tmp { other };
            std::swap(m_data, tmp.m_data);
            std::swap(m_capacity, tmp.m_capacity);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            m_data = std::move(other.m_data);
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            other.m_capacity = 0;
            other.m_size = 0;
        }
        return *this;
    }

    Vector& operator=(std::initializer_list<T> ilist) {
        Vector tmp { ilist };
        std::swap(m_data, tmp.m_data);
        std::swap(m_capacity, tmp.m_capacity);
        std::swap(m_size, tmp.m_size);
        return *this;
    }

    void reserve(std::size_t n) override {
        if (n > m_capacity) {
            auto tmp { std::make_unique<T[]>(n) };
            std::move(m_data.get(), m_data.get() + m_size, tmp.get());
            m_data = std::move(tmp);
            m_capacity = n;
        }
    }

    void resize(std::size_t n) override {
        if (n > m_capacity) {
            reserve(n);
        }
        m_size = n;
    }

    using iterator = AbstractVector<T>::iterator;
    using const_iterator = AbstractVector<T>::const_iterator;
    using AbstractVector<T>::begin;
    using AbstractVector<T>::end;

    iterator insert(iterator p, const T& e) override {
        if (m_size == m_capacity) {
            reserve(m_allocator(m_capacity, m_size));
        }
        std::move_backward(p, end(), end() + 1);
        *p = e;
        ++m_size;
        return p;
    }

    iterator insert(iterator p, T&& e) override {
        if (m_size == m_capacity) {
            reserve(m_allocator(m_capacity, m_size));
        }
        std::move_backward(p, end(), end() + 1);
        *p = std::move(e);
        ++m_size;
        return p;
    }

    iterator find(const T& e) const override {
        for (auto it { begin() }; it != end(); ++it) {
            if (*it == e) {
                return it;
            }
        }
        return end();
    }

    iterator erase(iterator p) override {
        std::move(p + 1, end(), p);
        --m_size;
        return p;
    }

    std::string type_name() const override {
        return std::format("Vector [{}]", m_allocator.type_name());
    }

};

}