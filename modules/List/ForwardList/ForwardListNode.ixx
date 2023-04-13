module;
#include <memory>

export module ForwardList.ForwardListNode;

import Framework.PointerProxy;

export namespace dslab {

template <typename T>
class ForwardListNode;

template <typename T>
using ForwardListNodeInst = UniquePointerProxy<ForwardListNode<T>>;

template <typename T>
using ForwardListNodePos = RawPointerProxy<ForwardListNode<T>>;

template <typename T>
using ForwardListNodeConstPos = ConstRawPointerProxy<ForwardListNode<T>>;

template <typename T>
class ForwardListNode {
    T m_data;
    ForwardListNodeInst<T> m_next {};

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
    ForwardListNodeInst<T>& next() { return m_next; }
    const ForwardListNodeInst<T>& next() const { return m_next; }
};

}