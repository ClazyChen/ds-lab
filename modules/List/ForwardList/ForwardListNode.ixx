module;
#include <memory>

export module ForwardList.ForwardListNode;

export namespace dslab {

template <typename T>
class ForwardListNode {
    T m_data;
    std::unique_ptr<ForwardListNode> m_next { nullptr };

    class ForwardListNodeProxy {
        std::unique_ptr<ForwardListNode>& m_node;
    public:
        ForwardListNodeProxy(std::unique_ptr<ForwardListNode>& node) : m_node(node) {}
        ForwardListNodeProxy(const std::unique_ptr<ForwardListNode>& node) : m_node(const_cast<std::unique_ptr<ForwardListNode>&>(node)) {}
        ForwardListNodeProxy(const ForwardListNodeProxy&) = delete;
        ForwardListNodeProxy& operator=(const ForwardListNodeProxy&) = delete;
        ForwardListNodeProxy(ForwardListNodeProxy&&) = delete;
        
        ForwardListNodeProxy& operator=(ForwardListNodeProxy&& other) {
            m_node = std::move(other.m_node);
            return *this;
        }
        ForwardListNodeProxy& operator=(std::unique_ptr<ForwardListNode>&& node) {
            m_node = std::move(node);
            return *this;
        }
        ForwardListNodeProxy& operator=(ForwardListNode* node) {
            m_node.reset(node);
            return *this;
        }
        ForwardListNode* operator->() const {
            return m_node.get();
        }
        operator ForwardListNode* () const {
            return m_node.get();
        }
        operator bool() const {
            return m_node != nullptr;
        }
        ForwardListNode* get() const {
            return m_node.get();
        }
        ForwardListNode* release() {
            return m_node.release();
        }
        void reset(ForwardListNode* node = nullptr) {
            m_node.reset(node);
        }
        void swap(ForwardListNodeProxy& other) {
            m_node.swap(other.m_node);
        }
    };

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
    ForwardListNodeProxy next() { return m_next; }
    const ForwardListNodeProxy next() const { return m_next; }
};

}