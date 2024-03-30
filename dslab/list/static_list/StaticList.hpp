#pragma once

#include "AbstractStaticList.hpp"
#include "../../vector.hpp"

namespace dslab::list::static_list {

template <typename T>
class StaticList : public AbstractStaticList<T> {
    
    using typename AbstractStaticList<T>::node_type;
    using typename AbstractStaticList<T>::iterator;
    Vector<node_type> m_nodes {};
    Vector<std::size_t> m_free {};
    std::size_t m_size { 0 };

    static constexpr std::size_t NIL { 0 };
    static constexpr std::size_t HEAD { 1 };
    static constexpr std::size_t TAIL { 2 };
    static constexpr std::size_t META_SIZE { 3 };

    node_type& node(std::size_t index) override {
        return m_nodes[index];
    }

    const node_type& node(std::size_t index) const override {
        return m_nodes[index];
    }

    std::size_t head() const override {
        return HEAD;
    }

    std::size_t tail() const override {
        return TAIL;
    }

    iterator create() {
        ++m_size;
        if (m_free.empty()) {
            m_nodes.push_back(node_type {});
            return iterator { this, m_nodes.size() - 1 };
        } else {
            auto index { m_free.back() };
            m_free.pop_back();
            return iterator { this, index };
        }
    }

public:
    using AbstractStaticList<T>::begin;
    using AbstractStaticList<T>::end;

    std::size_t size() const override {
        return m_size;
    }

    StaticList() {
        clear();
    }
    StaticList(const StaticList& list) : m_nodes(list.m_nodes), m_free(list.m_free), m_size(list.m_size) {}
    StaticList(StaticList&& list) noexcept : m_nodes(std::move(list.m_nodes)), m_free(std::move(list.m_free)), m_size(list.m_size){
        list.clear();
    }

    StaticList& operator=(const StaticList& list) {
        if (this != &list) {
            auto tmp { list };
            std::swap(m_nodes, tmp.m_nodes);
            std::swap(m_free, tmp.m_free);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }

    StaticList& operator=(StaticList&& list) noexcept {
        if (this != &list) {
            auto tmp { std::move(list) };
            std::swap(m_nodes, tmp.m_nodes);
            std::swap(m_free, tmp.m_free);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }

    StaticList(std::initializer_list<T> ilist) : StaticList() {
        for (auto&& item : ilist) {
            this->push_back(item);
        }
    }

    StaticList& operator=(std::initializer_list<T> ilist) {
        StaticList tmp { ilist };
        *this = std::move(tmp);
        return *this;
    }

    virtual ~StaticList() = default;

    void clear() override {
        m_nodes.resize(META_SIZE);
        node(HEAD).next() = TAIL;
        node(TAIL).prev() = HEAD;
        m_free.clear();
        m_size = 0;
    }

    iterator insertAsNext(iterator p, const T& e) override {
        auto q { create() }, r { p + 1 };
        m_nodes[q.index()] = node_type { e, p.index(), r.index() };
        p.node().next() = q.index();
        r.node().prev() = q.index();
        return q;
    }

    iterator insertAsPrev(iterator p, const T& e) override {
        return insertAsNext(--p, e);
    }

    iterator insertAsNext(iterator p, T&& e) override {
        auto q { create() }, r { p + 1 };
        m_nodes[q.index()] = node_type { std::move(e), p.index(), r.index() };
        p.node().next() = q.index();
        r.node().prev() = q.index();
        return q;
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
        auto q { p - 1 }, r { p + 1 };
        q.node().next() = r.index();
        r.node().prev() = q.index();
        --m_size;
        m_free.push_back(p.index());
        return r;
    }

    std::string type_name() const override {
        return "List (Bidirectional Static)";
    }

};

}