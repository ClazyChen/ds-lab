export module List.ListNode;

import Framework.PointerProxy;
import std;

export namespace dslab {

template <typename T>
class ListNode;

template <typename T>
using ListNodeInst = UniquePointerProxy<ListNode<T>>;

template <typename T>
using ListNodePos = RawPointerProxy<ListNode<T>>;

template <typename T>
using ListNodeConstPos = ConstRawPointerProxy<ListNode<T>>;

template <typename T>
class ListNode {
    T m_data;
    ListNodePos<T> m_prev {};
    ListNodeInst<T> m_next {};

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
    ListNodePos<T>& prev() { return m_prev; }
    const ListNodePos<T>& prev() const { return m_prev; }
    ListNodeInst<T>& next() { return m_next; }
    const ListNodeInst<T>& next() const { return m_next; }
};

}