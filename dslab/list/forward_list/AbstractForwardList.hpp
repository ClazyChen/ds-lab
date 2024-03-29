#pragma once

#include "ForwardListIterator.hpp"
#include "ForwardListNode.hpp"
#include "../../vector/LinearList.hpp"

namespace dslab::list::forward_list {

template <typename T>
class AbstractForwardList : public dslab::vector::LinearList<T, ForwardListIterator<AbstractForwardList<T>>, ConstForwardListIterator<AbstractForwardList<T>>> {
protected:
    virtual ForwardListNode<T>* head() = 0;
    virtual const ForwardListNode<T>* head() const = 0;
    virtual ForwardListNode<T>* tail() = 0;
    virtual const ForwardListNode<T>* tail() const = 0;

public:
    using value_type = T;
    using node_type = ForwardListNode<T>;
    using iterator = ForwardListIterator<AbstractForwardList<T>>;
    using const_iterator = ConstForwardListIterator<AbstractForwardList<T>>;

    virtual iterator insertAsNext(iterator p, const T& e) = 0;
    virtual iterator insertAsNext(iterator p, T&& e) = 0;
    virtual iterator insertAsPrev(iterator p, const T& e) = 0;
    virtual iterator insertAsPrev(iterator p, T&& e) = 0;
    virtual iterator eraseAfter(iterator p) = 0;

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

    void push_front(const T& e) {
        insertAsNext(iterator { this, head() }, e);
    }
    void push_front(T&& e) {
        insertAsNext(iterator { this, head() }, std::move(e));
    }
    void pop_front() {
        eraseAfter(iterator { this, head() });
    }

private:
    [[noreturn]] void pop_back() {
        throw std::logic_error("AbstractForwardList::pop_back() is not implemented");
    }

    [[noreturn]] T& back() {
        throw std::logic_error("AbstractForwardList::back() is not implemented");
    }

    [[noreturn]] const T& back() const {
        throw std::logic_error("AbstractForwardList::back() is not implemented");
    }
};

}