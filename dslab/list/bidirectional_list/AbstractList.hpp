#pragma once

#include "ListIterator.hpp"
#include "ListNode.hpp"
#include "../../vector/LinearList.hpp"

namespace dslab::list::bidirectional_list {

template <typename T>
class AbstractList : public dslab::vector::LinearList<T, ListIterator<AbstractList<T>>, ConstListIterator<AbstractList<T>>> {
protected:
    virtual ListNode<T>* head() = 0;
    virtual const ListNode<T>* head() const = 0;
    virtual ListNode<T>* tail() = 0;
    virtual const ListNode<T>* tail() const = 0;

public:
    using value_type = T;
    using node_type = ListNode<T>;
    using iterator = ListIterator<AbstractList<T>>;
    using const_iterator = ConstListIterator<AbstractList<T>>;

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
        return iterator { this, head()->next().get() };
    }

    iterator end() noexcept override {
        return iterator { this, tail() };
    }

    const_iterator begin() const noexcept {
        return const_iterator { this, head()->next().get() };
    }

    const_iterator end() const noexcept {
        return const_iterator { this, tail() };
    }

    const_iterator cbegin() const noexcept {
        return const_iterator { this, head()->next().get() };
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