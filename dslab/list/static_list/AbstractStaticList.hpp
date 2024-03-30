#pragma once

#include "StaticListIterator.hpp"
#include "StaticListNode.hpp"
#include "../../vector/LinearList.hpp"

namespace dslab::list::static_list {

template <typename T>
class AbstractStaticList : public dslab::vector::LinearList<T, StaticListIterator<AbstractStaticList<T>>, ConstStaticListIterator<AbstractStaticList<T>>> {
protected:
    friend class StaticListIterator<AbstractStaticList<T>>;
    friend class ConstStaticListIterator<AbstractStaticList<T>>;
    virtual StaticListNode<T>& node(std::size_t index) = 0;
    virtual const StaticListNode<T>& node(std::size_t index) const = 0;

    virtual std::size_t head() const = 0;
    virtual std::size_t tail() const = 0;

public:
    using value_type = T;
    using node_type = StaticListNode<T>;
    using iterator = StaticListIterator<AbstractStaticList<T>>;
    using const_iterator = ConstStaticListIterator<AbstractStaticList<T>>;

    virtual iterator insertAsNext(iterator p, const T& e) = 0;
    virtual iterator insertAsNext(iterator p, T&& e) = 0;
    virtual iterator insertAsPrev(iterator p, const T& e) = 0;
    virtual iterator insertAsPrev(iterator p, T&& e) = 0;

    // insert e means insert e as the previous element of p
    iterator insert(iterator p, const T& e) override {
        return insertAsPrev(p, e);
    }

    iterator insert(iterator p, T&& e) override {
        return insertAsPrev(p, std::move(e));
    }

    // iterator-related methods below

    iterator begin() noexcept override {
        return iterator { this, node(head()).next() };
    }

    iterator end() noexcept override {
        return iterator { this, tail() };
    }

    const_iterator begin() const noexcept {
        return const_iterator { this, node(head()).next() };
    }

    const_iterator end() const noexcept {
        return const_iterator { this, tail() };
    }

    const_iterator cbegin() const noexcept {
        return const_iterator { this, node(head()).next() };
    }

    const_iterator cend() const noexcept {
        return const_iterator { this, tail() };
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