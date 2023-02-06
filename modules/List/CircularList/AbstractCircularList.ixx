module;
#include <iostream>
#include <iterator>
#include <stdexcept>

export module List.CircularList.AbstractCircularList;

import LinearList;
import List.ListNode;
import List.AbstractList;
import List.ListIterator;

export namespace dslab {

template <typename T>
class AbstractCircularList : public AbstractList<T> {
public:
    ListNode<T>* insert(ListNode<T>* p, const T& e) override {
        return this->insertAsPrev(p, e);
    }

    ListNode<T>* insert(ListNode<T>* p, T&& e) override {
        return this->insertAsPrev(p, std::move(e));
    }

    T& get(ListNode<T>* p) override {
        return p->data();
    }

    void set(ListNode<T>* p, const T& e) override {
        p->data() = e;
    }

    void set(ListNode<T>* p, T&& e) override {
        p->data() = std::move(e);
    }

    ListNode<T>* first() const override {
        return const_cast<AbstractCircularList*>(this)->head();
    }

    ListNode<T>* last() const override {
        if (auto f { first() }; f) {
            return f->prev();
        } else {
            return nullptr;
        }
    }

    ListNode<T>* next(ListNode<T>* p) const override {
        return p->next().get();
    }

    ListNode<T>* prev(ListNode<T>* p) const override {
        return p->prev();
    }

private:
    [[noreturn]] bool end(ListNode<T>* p) const override {
        throw std::logic_error("Circular List does not have an end");
    }

    [[noreturn]] ListNode<T>* tail() override {
        throw std::logic_error("Circular List does not have a tail");
    }

public:
    void push_back(const T& e) override {
        this->insertAsPrev(this->head(), e);
    }

    void push_back(T&& e) override {
        this->insertAsPrev(this->head(), std::move(e));
    }

    void push_front(const T& e) override {
        this->insertAsPrev(this->head(), e);
    }

    void push_front(T&& e) override {
        this->insertAsPrev(this->head(), std::move(e));
    }

    using iterator = ListIterator<AbstractList<T>>;
    using const_iterator = ConstListIterator<AbstractList<T>>;

    iterator begin() noexcept {
        return iterator { this, first() };
    }

    iterator end() noexcept {
        return iterator { this, nullptr };
    }

    const_iterator begin() const noexcept {
        return const_iterator { this, first() };
    }

    const_iterator end() const noexcept {
        return const_iterator { this, nullptr };
    }

    const_iterator cbegin() const noexcept {
        return const_iterator { this, first() };
    }

    const_iterator cend() const noexcept {
        return const_iterator { this, nullptr };
    }

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    reverse_iterator rbegin() noexcept {
        return reverse_iterator { iterator { this, last() } };
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator { iterator { this, nullptr } };
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator { const_iterator { this, last() } };
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator { const_iterator { this, nullptr } };
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator { const_iterator { this, last() } };
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator { const_iterator { this, nullptr } };
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractCircularList<T>& list) {
    os << "CL[";
    if (list.size() > 0) {
        auto p { list.first() };
        do {
            os << p->data() << ", ";
            p = p->next().get();
        } while (p != list.first());
        os << "\b\b";
    }
    os << "]";
    return os;
}

}