module;
#include <memory>

export module List.ListNode;

export namespace dslab {

template <typename T>
class ListNode {
    T m_data;
    ListNode* m_prev { nullptr };
    std::unique_ptr<ListNode> m_next { nullptr };
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
    ListNode*& prev() { return m_prev; }
    const ListNode*& prev() const { return m_prev; }
    std::unique_ptr<ListNode>& next() { return m_next; }
    const std::unique_ptr<ListNode>& next() const { return m_next; }
};

}