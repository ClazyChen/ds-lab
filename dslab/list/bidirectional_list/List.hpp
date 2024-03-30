#pragma once

#include "AbstractList.hpp"

namespace dslab::list::bidirectional_list {

template <typename T>
class List : public AbstractList<T> {
    std::unique_ptr<ListNode<T>> m_head { nullptr };
    ListNode<T>* m_tail { nullptr };
    std::size_t m_size { 0 };

    void initialize() {
        m_head = std::make_unique<ListNode<T>>();
        m_head->next() = std::make_unique<ListNode<T>>();
        m_tail = m_head->next().get();
        m_tail->prev() = m_head.get();
        m_size = 0;
    }

protected:
    ListNode<T>* head() override { return m_head.get(); }
    const ListNode<T>* head() const override { return m_head.get(); }
    ListNode<T>* tail() override { return m_tail; }
    const ListNode<T>* tail() const override { return m_tail; }

public:
    using iterator = AbstractList<T>::iterator;
    using AbstractList<T>::begin;
    using AbstractList<T>::end;
    using AbstractList<T>::insert;

    std::size_t size() const override { return m_size; }
    List() { initialize(); }
    List(const List& other) : List() {
        for (auto&& item : other) {
            insert(end(), item);
        }
    }
    List(List&& other) noexcept {
        m_head = std::move(other.m_head);
        m_tail = other.m_tail;
        m_size = other.m_size;
        other.initialize();
    }
    List& operator=(const List& other) {
        if (this != &other) {
            List tmp { other };
            std::swap(m_head, tmp.m_head);
            std::swap(m_tail, tmp.m_tail);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    List& operator=(List&& other) noexcept {
        if (this != &other) {
            List tmp { std::move(other) };
            std::swap(m_head, tmp.m_head);
            std::swap(m_tail, tmp.m_tail);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    List(std::initializer_list<T> ilist) : List() {
        for (auto&& item : ilist) {
            insert(end(), item);    
        }
    }
    List& operator=(std::initializer_list<T> ilist) {
        List tmp { ilist };
        std::swap(m_head, tmp.m_head);
        std::swap(m_tail, tmp.m_tail);
        std::swap(m_size, tmp.m_size);
        return *this;
    }

    virtual ~List() = default;

    void clear() override {
        m_head->next() = std::make_unique<ListNode<T>>();
        m_tail = m_head->next().get();
        m_tail->prev() = m_head.get();
        m_size = 0;
    }

    iterator insertAsNext(iterator p, const T& e) override {
        auto node { std::make_unique<ListNode<T>>(e) };
        node->next() = std::move(p.node()->next());
        node->next()->prev() = node.get();
        node->prev() = p.node();
        p.node()->next() = std::move(node);
        ++m_size;
        return ++p;
    }

    iterator insertAsPrev(iterator p, const T& e) override {
        return insertAsNext(--p, e);
    }

    iterator insertAsNext(iterator p, T&& e) override {
        auto node { std::make_unique<ListNode<T>>(std::move(e)) };
        node->next() = std::move(p.node()->next());
        node->next()->prev() = node.get();
        node->prev() = p.node();
        p.node()->next() = std::move(node);
        ++m_size;
        return ++p;
    }

    iterator insertAsPrev(iterator p, T&& e) override {
        return insertAsNext(--p, std::move(e));
    }

    iterator find(const T& e) const override {
        auto p { begin() };
        while (p != end() && *p != e) {
            ++p;
        }
        return p;
    }

    iterator erase(iterator p) override {
        auto node { p.node() }, q { p + 1 };
        node->next()->prev() = node->prev();
        node->prev()->next() = std::move(node->next());
        --m_size;
        return q;
    }

    std::string type_name() const override {
        return "List (Bidirectional)";
    }
    
};

}