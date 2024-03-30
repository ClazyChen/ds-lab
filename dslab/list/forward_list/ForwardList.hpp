#pragma once

#include "AbstractForwardList.hpp"

namespace dslab::list::forward_list {

template <typename T>
class ForwardList : public AbstractForwardList<T> {
    std::unique_ptr<ForwardListNode<T>> m_head { nullptr };
    ForwardListNode<T>* m_tail { nullptr };
    std::size_t m_size { 0 };

    void initialize() {
        m_head = std::make_unique<ForwardListNode<T>>();
        m_head->next() = std::make_unique<ForwardListNode<T>>();
        m_tail = m_head->next().get();
        m_size = 0;
    }

protected:
    ForwardListNode<T>* head() override { return m_head.get(); }
    const ForwardListNode<T>* head() const override { return m_head.get(); }
    ForwardListNode<T>* tail() override { return m_tail; }
    const ForwardListNode<T>* tail() const override { return m_tail; }

public:
    using iterator = AbstractForwardList<T>::iterator;
    using AbstractForwardList<T>::begin;
    using AbstractForwardList<T>::end;

    std::size_t size() const override { return m_size; }

    ForwardList() { initialize(); }
    ForwardList(const ForwardList& other) : ForwardList() {
        iterator p { this, head() };
        for (auto&& item : other) {
            p = insertAsNext(p, item);
        }
    }
    ForwardList(ForwardList&& other) noexcept {
        m_head = std::move(other.m_head);
        m_tail = other.m_tail;
        m_size = other.m_size;
        other.initialize();
    }
    ForwardList& operator=(const ForwardList& other) {
        if (this != &other) {
            ForwardList tmp { other };
            std::swap(m_head, tmp.m_head);
            std::swap(m_tail, tmp.m_tail);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    ForwardList& operator=(ForwardList&& other) noexcept {
        if (this != &other) {
            ForwardList tmp { std::move(other) };
            std::swap(m_head, tmp.m_head);
            std::swap(m_tail, tmp.m_tail);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    ForwardList(std::initializer_list<T> ilist) : ForwardList() {
        iterator p { this, head() };
        for (auto&& item : ilist) {
            p = insertAsNext(p, item);
        }
    }
    ForwardList& operator=(std::initializer_list<T> ilist) {
        ForwardList tmp { ilist };
        std::swap(m_head, tmp.m_head);
        std::swap(m_tail, tmp.m_tail);
        std::swap(m_size, tmp.m_size);
        return *this;
    }

    virtual ~ForwardList() = default;

    void clear() override {
        m_head->next() = std::make_unique<ForwardListNode<T>>();
        m_tail = m_head->next().get();
        m_size = 0;
    }

    iterator insertAsNext(iterator p, const T& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(e) };
        node->next() = std::move(p.node()->next());
        p.node()->next() = std::move(node);
        ++m_size;
        return ++p;
    }

    iterator insertAsNext(iterator p, T&& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(std::move(e)) };
        node->next() = std::move(p.node()->next());
        p.node()->next() = std::move(node);
        ++m_size;
        return ++p;
    }

    iterator insertAsPrev(iterator p, const T& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(std::move(*p)) };
        *p = e;
        node->next() = std::move(p.node()->next());
        if (p == end()) {
            m_tail = node.get();
        }
        p.node()->next() = std::move(node);
        ++m_size;
        return p;
    }

    iterator insertAsPrev(iterator p, T&& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(std::move(*p)) };
        *p = std::move(e);
        node->next() = std::move(p.node()->next());
        if (p == end()) {
            m_tail = node.get();
        }
        p.node()->next() = std::move(node);
        ++m_size;
        return p;
    }

    iterator find(const T& e) const override {
        auto p { begin() };
        while (p != end() && *p != e) {
            ++p;
        }
        return p;
    }

    iterator erase(iterator p) override {
        auto q { p + 1 };
        *p = std::move(*q);
        return eraseAfter(p);
    }

    iterator eraseAfter(iterator p) override {
        auto q { p + 1 };
        p.node()->next() = std::move(q.node()->next());
        if (q == end()) {
            m_tail = p.node();
        }
        --m_size;
        return p;
    }

    std::string type_name() const override {
        return "List (Forward)";
    }
};

}