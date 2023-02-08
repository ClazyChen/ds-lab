module;
#include <memory>

export module List.StaticList.StaticListNode;

using Rank = size_t;

export namespace dslab {

template <typename T>
class StaticListNode {
    T m_data;
    Rank m_prev { 0 };
    Rank m_next { 0 };
public:
    StaticListNode() = default;
    StaticListNode(const T& data) : m_data(data) {}
    StaticListNode(T&& data) : m_data(std::move(data)) {}
    StaticListNode(const T& data, Rank prev, Rank next) : m_data(data), m_prev(prev), m_next(next) {}
    StaticListNode(T&& data, Rank prev, Rank next) : m_data(std::move(data)), m_prev(prev), m_next(next) {}
    StaticListNode(const StaticListNode& node) : m_data(node.m_data), m_prev(node.m_prev), m_next(node.m_next) {}
    StaticListNode(StaticListNode&& node) noexcept : m_data(std::move(node.m_data)), m_prev(node.m_prev), m_next(node.m_next) {}
    StaticListNode& operator=(const StaticListNode& node) {
        if (this != &node) {
            m_data = node.m_data;
            m_prev = node.m_prev;
            m_next = node.m_next;
        }
        return *this;
    }
    StaticListNode& operator=(StaticListNode&& node) noexcept {
        if (this != &node) {
            m_data = std::move(node.m_data);
            m_prev = node.m_prev;
            m_next = node.m_next;
        }
        return *this;
    }
    T& data() { return m_data; }
    const T& data() const { return m_data; }
    Rank& prev() { return m_prev; }
    const Rank& prev() const { return m_prev; }
    Rank& next() { return m_next; }
    const Rank& next() const { return m_next; }

    bool operator==(const StaticListNode& node) const { return m_data == node.m_data; }
    auto operator<=>(const StaticListNode& node) const { return m_data <=> node.m_data; }
};

}