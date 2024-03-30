#pragma once

#include "AbstractCircularList.hpp"
#include <ranges>

namespace dslab::list::circular_list {

template <typename T>
class CircularList : public AbstractCircularList<T> {
    ListNode<T>* m_head { nullptr };
    std::size_t m_size { 0 };

protected:
    ListNode<T>* head() override { return m_head; }
    const ListNode<T>* head() const override { return m_head; }

public:
    using iterator = AbstractList<T>::iterator;
    using AbstractCircularList<T>::begin;
    using AbstractCircularList<T>::end;

    std::size_t size() const override { return m_size; }

    CircularList() = default;
    CircularList(const CircularList& other) : CircularList() {
        for (auto& item : std::views::counted(other.begin(), other.size())) {
            this->push_back(item);
        }
    }
    CircularList(CircularList&& other) noexcept {
        m_head = other.m_head;
        m_size = other.m_size;
        other.m_head = nullptr;
        other.m_size = 0;
    }
    CircularList& operator=(const CircularList& other) {
        if (this != &other) {
            CircularList tmp { other };
            std::swap(m_head, tmp.m_head);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    CircularList& operator=(CircularList&& other) noexcept {
        if (this != &other) {
            CircularList tmp { std::move(other) };
            std::swap(m_head, tmp.m_head);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    CircularList(std::initializer_list<T> ilist) : CircularList() {
        for (auto&& item : ilist) {
            this->push_back(item);
        }
    }
    CircularList& operator=(std::initializer_list<T> ilist) {
        CircularList tmp { ilist };
        std::swap(m_head, tmp.m_head);
        std::swap(m_size, tmp.m_size);
        return *this;
    }

    void clear() override {
        if (head()) {
            head()->next().reset();
        }
        m_head = nullptr;
        m_size = 0;
    }

    virtual ~CircularList() {
        clear();
    }

    iterator insertAsNext(iterator p, const T& e) override {
        auto node { std::make_unique<ListNode<T>>(e) };
        if (p == end()) {
            m_head = node.get();
            node->prev() = node.get();
            node->next() = std::move(node);
            p = begin();
        } else {
            node->next() = std::move(p.node()->next());
            node->next()->prev() = node.get();
            node->prev() = p.node();
            p.node()->next() = std::move(node);
        }
        ++m_size;
        return ++p;
    }

    iterator insertAsPrev(iterator p, const T& e) override {
        auto q { p == end() ? p : p - 1 };
        auto r { insertAsNext(q, e) };
        if (p == begin()) {
            m_head = r.node();
        }
        return r;
    }

    iterator insertAsNext(iterator p, T&& e) override {
        auto node { std::make_unique<ListNode<T>>(std::move(e)) };
        if (p == end()) {
            m_head = node.get();
            node->prev() = node.get();
            node->next() = std::move(node);
            p = begin();
        } else {
            node->next() = std::move(p.node()->next());
            node->next()->prev() = node.get();
            node->prev() = p.node();
            p.node()->next() = std::move(node);
        }
        ++m_size;
        return ++p;
    }

    iterator insertAsPrev(iterator p, T&& e) override {
        auto q { p == end() ? p : p - 1 };
        auto r { insertAsNext(q, std::move(e)) };
        if (p == begin()) {
            m_head = r.node();
        }
        return r;
    }

    iterator find(const T& e) const override {
        auto p { begin() };
        while (p != end() && *p != e) {
            if (++p == begin()) {
                return end();
            }
        }
        return p;
    }

    iterator erase(iterator p) override {
        if (size() == 1) {
            clear();
            return end();
        } else {
            auto q { p + 1 }; 
            p.node()->next()->prev() = p.node()->prev();
            if (p == begin()) {
                m_head = (++begin()).node();
            }
            p.node()->prev()->next() = std::move(p.node()->next());
            --m_size;
            return q;
        }
    }

    std::string type_name() const override {
        return "List (Bidirectional Circular)";
    }
};

}