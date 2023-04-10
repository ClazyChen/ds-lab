module;
#include <memory>

export module List.ListNode;

import Framework.PointerProxy;

export namespace dslab {

template <typename T>
class ListNode {
    T m_data;
    
    using ListNodeProxy = UniquePointerProxy<ListNode<T>>;
    using ListNodeRawProxy = RawPointerProxy<ListNode<T>>;

    ListNodeRawProxy m_prev {};
    ListNodeProxy m_next {};

public:
    ListNode() = default;
    ListNode(const T& data) : m_data(data) {}
    ListNode(T&& data) : m_data(std::move(data)) {}

    virtual ~ListNode() {
        auto p { std::move(m_next) };
        while (p != nullptr) {
            p = std::move(p->m_next);
        }
    }

    ListNode(const ListNode&) = delete;
    ListNode& operator=(const ListNode&) = delete;
    ListNode(ListNode&&) = delete;
    ListNode& operator=(ListNode&&) = delete;

    T& data() { return m_data; }
    const T& data() const { return m_data; }
    ListNodeRawProxy& prev() { return m_prev; }
    const ListNodeRawProxy& prev() const { return m_prev; }
    ListNodeProxy& next() { return m_next; }
    const ListNodeProxy& next() const { return m_next; }
};

}