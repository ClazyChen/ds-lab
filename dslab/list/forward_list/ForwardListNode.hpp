#pragma once

#include <memory>

namespace dslab::list::forward_list {

template <typename T>
class ForwardListNode {
    T m_data;
    std::unique_ptr<ForwardListNode<T>> m_next {};

public:
    ForwardListNode() = default;
    ForwardListNode(const T& data) : m_data(data) {}
    ForwardListNode(T&& data) : m_data(std::move(data)) {}

    virtual ~ForwardListNode() {
        auto p { std::move(m_next) };
        while (p != nullptr) {
            p = std::move(p->m_next);
        }
    }

    ForwardListNode(const ForwardListNode&) = delete;
    ForwardListNode& operator=(const ForwardListNode&) = delete;
    ForwardListNode(ForwardListNode&&) = delete;
    ForwardListNode& operator=(ForwardListNode&&) = delete;

    T& data() { return m_data; }
    const T& data() const { return m_data; }
    std::unique_ptr<ForwardListNode<T>>& next() { return m_next; }
    const std::unique_ptr<ForwardListNode<T>>& next() const { return m_next; }

};

}