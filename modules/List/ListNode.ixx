module;
#include <memory>

export module List.ListNode;

export namespace dslab {

template <typename T>
class ListNode {
    T m_data;
    ListNode* m_prev { nullptr };
    std::unique_ptr<ListNode> m_next { nullptr };
    
    class ListNodeProxy;
    class ListNodeRawProxy {
        ListNode*& m_node;
    public:
        ListNodeRawProxy(ListNode*& node) : m_node(node) {}
        ListNodeRawProxy(const ListNode*& node) : m_node(const_cast<ListNode*&>(node)) {}
        ListNodeRawProxy(const ListNodeRawProxy&) = delete;
        ListNodeRawProxy(ListNodeRawProxy&&) = delete;

        ListNodeRawProxy& operator=(const ListNodeRawProxy& other) {
            m_node = other.m_node;
            return *this;
        }
        ListNodeRawProxy& operator=(const ListNodeProxy& other) {
            m_node = other.get();
            return *this;
        }
        ListNodeRawProxy& operator=(ListNode* node) {
            m_node = node;
            return *this;
        }
        ListNodeRawProxy& operator=(const std::unique_ptr<ListNode>& node) {
            m_node = node.get();
            return *this;
        }
        ListNode* operator->() const {
            return m_node;
        }
        operator ListNode* () const {
            return m_node;
        }
        operator bool() const {
            return m_node != nullptr;
        }
        void swap(ListNodeRawProxy& other) {
            std::swap(m_node, other.m_node);
        }
    };

    class ListNodeProxy {
        std::unique_ptr<ListNode>& m_node;
    public:
        ListNodeProxy(std::unique_ptr<ListNode>& node) : m_node(node) {}
        ListNodeProxy(const std::unique_ptr<ListNode>& node) : m_node(const_cast<std::unique_ptr<ListNode>&>(node)) {}
        ListNodeProxy(const ListNodeProxy&) = delete;
        ListNodeProxy& operator=(const ListNodeProxy&) = delete;
        ListNodeProxy(ListNodeProxy&&) = delete;

        ListNodeProxy& operator=(ListNodeProxy&& other) {
            m_node = std::move(other.m_node);
            return *this;
        }
        ListNodeProxy& operator=(std::unique_ptr<ListNode>&& node) {
            m_node = std::move(node);
            return *this;
        }
        ListNodeProxy& operator=(ListNode* node) {
            m_node.reset(node);
            return *this;
        }
        ListNode* operator->() const {
            return m_node.get();
        }
        operator ListNode* () const {
            return m_node.get();
        }
        operator bool() const {
            return m_node != nullptr;
        }
        ListNode* get() const {
            return m_node.get();
        }
        ListNode* release() {
            return m_node.release();
        }
        void reset(ListNode* node = nullptr) {
            m_node.reset(node);
        }
        void swap(ListNodeProxy& other) {
            m_node.swap(other.m_node);
        }
    };

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
    ListNodeRawProxy prev() { return m_prev; }
    const ListNodeRawProxy prev() const { return m_prev; }
    ListNodeProxy next() { return m_next; }
    const ListNodeProxy next() const { return m_next; }
};

}