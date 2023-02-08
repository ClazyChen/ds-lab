module;
#include <iostream>
#include <iterator>
#include <stdexcept>

export module ForwardList.AbstractForwardList;

import LinearList;
import ForwardList.ForwardListNode;
import ForwardList.ForwardListIterator;

export namespace dslab {

template <typename T>
class AbstractForwardList : public AbstractLinearList<T, ForwardListNode<T>*> {
public:
    virtual ForwardListNode<T>* head() = 0;
    virtual ForwardListNode<T>* tail() = 0;
    virtual const ForwardListNode<T>* head() const = 0;
    virtual const ForwardListNode<T>* tail() const = 0;
    virtual size_t size() const = 0;
    virtual ForwardListNode<T>* insertAsNext(ForwardListNode<T>* p, const T& e) = 0;
    virtual ForwardListNode<T>* insertAsNext(ForwardListNode<T>* p, T&& e) = 0;
    virtual ForwardListNode<T>* insertAsPrev(ForwardListNode<T>* p, const T& e) = 0;
    virtual ForwardListNode<T>* insertAsPrev(ForwardListNode<T>* p, T&& e) = 0;
    virtual ForwardListNode<T>* find(const T& e) const = 0;
    virtual T remove(ForwardListNode<T>* p) = 0;
    virtual void clear() = 0;

    ForwardListNode<T>* insert(ForwardListNode<T>* p, const T& e) override {
        return insertAsPrev(p, e);
    }

    ForwardListNode<T>* insert(ForwardListNode<T>* p, T&& e) override {
        return insertAsPrev(p, std::move(e));
    }

    T& get(ForwardListNode<T>* p) override {
        return p->data();
    }

    void set(ForwardListNode<T>* p, const T& e) override {
        p->data() = e;
    }

    void set(ForwardListNode<T>* p, T&& e) override {
        p->data() = std::move(e);
    }

    ForwardListNode<T>* first() const override {
        return head()->next().get();
    }

    ForwardListNode<T>* next(ForwardListNode<T>* p) const override {
        return p->next().get();
    }

private:
    // 单向链表不支持从后向前的访问
    [[noreturn]] ForwardListNode<T>* last() const override {
        throw std::logic_error("AbstractForwardList::last() is not implemented");
    }

    [[noreturn]] ForwardListNode<T>* prev(ForwardListNode<T>* p) const override {
        throw std::logic_error("AbstractForwardList::prev() is not implemented");
    }

    [[noreturn]] T pop_back() override {
        throw std::logic_error("AbstractForwardList::pop_back() is not implemented");
    }

    [[noreturn]] T& back() override {
        throw std::logic_error("AbstractForwardList::back() is not implemented");
    }

    [[noreturn]] const T& back() const override {
        throw std::logic_error("AbstractForwardList::back() is not implemented");
    }

public:
    bool end(ForwardListNode<T>* p) const override {
        return p == tail();
    }

    void push_back(const T& e) override {
        insertAsPrev(tail(), e);
    }

    void push_back(T&& e) override {
        insertAsPrev(tail(), std::move(e));
    }

    void push_front(const T& e) override {
        insertAsNext(head(), e);
    }

    void push_front(T&& e) override {
        insertAsNext(head(), std::move(e));
    }

    using iterator = ForwardListIterator<AbstractForwardList<T>>;
    using const_iterator = ConstForwardListIterator<AbstractForwardList<T>>;

    iterator begin() noexcept {
        return iterator { this, first() };
    }

    iterator end() noexcept {
        return iterator { this, tail() };
    }

    const_iterator begin() const noexcept {
        return const_iterator { this, first() };
    }

    const_iterator end() const noexcept {
        return const_iterator { this, const_cast<AbstractForwardList*>(this)->tail() };
    }

    const_iterator cbegin() const noexcept {
        return const_iterator { this, first() };
    }

    const_iterator cend() const noexcept {
        return const_iterator { this, const_cast<AbstractForwardList*>(this)->tail() };
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractForwardList<T>& list) {
    os << "FL[";
    for (auto it { list.begin() }; it != list.end(); ++it) {
        os << *it << ", ";
    }
    if (list.size() > 0) {
        os << "\b\b";
    }
    os << "]";
    return os;
}

}