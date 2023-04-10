module;
#include <memory>

export module ForwardList.ForwardListNode;

import Framework.PointerProxy;

export namespace dslab {

template <typename T>
class ForwardListNode {
    T m_data;

    using ForwardListNodeProxy = UniquePointerProxy<ForwardListNode<T>>;

    ForwardListNodeProxy m_next {};

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
    ForwardListNodeProxy& next() { return m_next; }
    const ForwardListNodeProxy& next() const { return m_next; }
};

}