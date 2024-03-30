#pragma once

#include "../../list.hpp"

namespace dslab::list::circular_list {

template <typename T>
class AbstractCircularList : public AbstractList<T> {
public:
    using AbstractList<T>::insertAsNext;
    using AbstractList<T>::empty;
    using AbstractList<T>::head;
    using AbstractList<T>::erase;
    
    void push_back(const T& e) {
        if (empty()) {
            insertAsNext(begin(), e);
        } else {
            insertAsNext(--begin(), e);
        }
    }

    void push_back(T&& e) {
        if (empty()) {
            insertAsNext(begin(), std::move(e));
        } else {
            insertAsNext(--begin(), std::move(e));
        }
    }

    void pop_back() {
        erase(--begin());
    }

    T& back() {
        return *--begin();
    }

    const T& back() const {
        return *--cbegin();
    }

    using iterator = AbstractList<T>::iterator;
    using const_iterator = AbstractList<T>::const_iterator;

    iterator begin() noexcept {
        return iterator { this, head() };
    }

    iterator end() noexcept {
        return iterator { this, nullptr };
    }

    const_iterator begin() const noexcept {
        return const_iterator { this, head() };
    }

    const_iterator end() const noexcept {
        return const_iterator { this, nullptr };
    }

    const_iterator cbegin() const noexcept {
        return const_iterator { this, head() };
    }

    const_iterator cend() const noexcept {
        return const_iterator { this, nullptr };
    }

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    reverse_iterator rbegin() noexcept {
        return reverse_iterator { iterator { this, head() } };
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator { iterator { this, nullptr } };
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator { const_iterator { this, head() } };
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator { const_iterator { this, nullptr } };
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator { const_iterator { this, head() } };
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator { const_iterator { this, nullptr } };
    }

private:
    [[noreturn]] ListNode<T>* tail() override {
        throw std::logic_error("Circular List does not have a tail");
    }

    [[noreturn]] const ListNode<T>* tail() const override {
        throw std::logic_error("Circular List does not have a tail");
    }
};

}