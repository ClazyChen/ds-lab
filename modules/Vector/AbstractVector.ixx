module;
#include <iterator>
#include <iostream>

export module Vector.AbstractVector;

import LinearList;
import Vector.VectorIterator;

export namespace dslab {

using Rank = size_t;

template <typename T>
class AbstractVector : public AbstractLinearList<T, Rank> {
protected:
    virtual T* data() = 0; // 返回向量的首地址
    virtual const T* data() const {
        return const_cast<AbstractVector*>(this)->data();
    }
public:
    virtual size_t capacity() const = 0; // 返回向量的容量
    virtual void reserve(size_t n) = 0; // 申请容量为n的向量
    virtual size_t size() const = 0; // 返回向量的规模
    virtual void resize(size_t n) = 0; // 重置向量的规模为n

    T& get(Rank r) override {
        return data()[r];
    }

    void set(Rank r, const T& e) override {
        data()[r] = e;
    }

    void set(Rank r, T&& e) override {
        data()[r] = std::move(e);
    }

    void clear() override {
        resize(0);
    }

    Rank first() const override {
        return 0;
    }

    Rank last() const override {
        return size() - 1;
    }

    Rank next(Rank r) const override {
        return r + 1;
    }

    Rank prev(Rank r) const override {
        return r - 1;
    }

    bool end(Rank r) const override {
        return r >= size();
    }

    using iterator = VectorIterator<AbstractVector>;
    using const_iterator = ConstVectorIterator<AbstractVector>;

    iterator begin() noexcept {
        return iterator { this, 0 };
    }

    iterator end() noexcept {
        return iterator { this, size() };
    }

    const_iterator begin() const noexcept {
        return const_iterator { this, 0 };
    }

    const_iterator end() const noexcept {
        return const_iterator { this, size() };
    }

    const_iterator cbegin() const noexcept {
        return const_iterator { this, 0 };
    }

    const_iterator cend() const noexcept {
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

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractVector<T>& v) {
    os << "V[";
    for (auto it { v.begin() }; it != v.end(); ++it) {
        os << *it << ", ";
    }
    if (v.size() > 0) {
        os << "\b\b";
    }
    os << "]";
    return os;
}

}