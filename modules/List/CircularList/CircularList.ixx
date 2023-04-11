module;
#include <memory>
#include <ranges>

export module List.CircularList;

export import List.CircularList.AbstractCircularList;
export import List.ListNode;

export namespace dslab {

template <typename T>
class CircularList : public AbstractCircularList<T> {
    ListNodePos<T> m_head { nullptr };
    size_t m_size { 0 };

public:
    ListNodePos<T> head() override { return m_head; }
    ListNodeConstPos<T> head() const override { return m_head; }
    size_t size() const override { return m_size; }

    CircularList() = default;
    CircularList(const CircularList& list) : CircularList() {
        for (auto& item : std::views::counted(std::begin(list), list.size())) {
            this->push_back(item);
        }
    }
    CircularList(CircularList&& list) noexcept {
        m_head = list.m_head;
        m_size = list.m_size;
        list.m_head = nullptr;
        list.m_size = 0;
    }
    CircularList& operator=(const CircularList& list) {
        if (this != &list) {
            CircularList tmp { list };
            std::swap(m_head, tmp.m_head);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    CircularList& operator=(CircularList&& list) noexcept {
        if (this != &list) {
            CircularList tmp { std::move(list) };
            std::swap(m_head, tmp.m_head);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }

    CircularList(std::initializer_list<T> ilist) : CircularList() {
        for (auto&& item : ilist) {
            this->push_back(item);
        }
    }
    CircularList& operator=(std::initializer_list<T> ilist) {
        CircularList tmp { ilist };
        std::swap(m_head, tmp.m_head);
        std::swap(m_size, tmp.m_size);
        return *this;
    }

    void clear() override {
        if (m_head) {
            m_head->next().reset();
        }
        m_size = 0;
    }

    virtual ~CircularList() {
        clear();
    }

    ListNodePos<T> insertAsNext(ListNodePos<T> p, const T& e) override {
        auto node { ListNodeInst<T>::make(e) };
        if (m_head == nullptr) {
            m_head = node;
            node->prev() = node;
            node->next() = std::move(node);
            p = m_head;
        } else {
            node->next() = std::move(p->next());
            node->next()->prev() = node;
            node->prev() = p;
            p->next() = std::move(node);
        }
        ++m_size;
        return p->next();
    }

    ListNodePos<T> insertAsPrev(ListNodePos<T> p, const T& e) override {
        auto q { p ? p->prev() : nullptr };
        auto node { insertAsNext(q, e) };
        if (p == m_head) {
            m_head = node;
        }
        return node;
    }

    ListNodePos<T> insertAsNext(ListNodePos<T> p, T&& e) override {
        auto node { std::make_unique<ListNode<T>>(std::move(e)) };
        if (m_head == nullptr) {
            m_head = node;
            node->prev() = node;
            node->next() = std::move(node);
            p = m_head;
        } else {
            node->next() = std::move(p->next());
            node->next()->prev() = node;
            node->prev() = p;
            p->next() = std::move(node);
        }
        ++m_size;
        return p->next();
    }

    ListNodePos<T> insertAsPrev(ListNodePos<T> p, T&& e) override {
        auto q { p ? p->prev() : nullptr };
        auto node { insertAsNext(q, std::move(e)) };
        if (p == m_head) {
            m_head = node;
        }
        return node;
    }

    ListNodePos<T> find(const T& e) const override {
        auto p { m_head };
        for (size_t i { 0 }; i < m_size; ++i) {
            if (p->data() == e) {
                return p;
            }
            p = p->next();
        }
        return nullptr;
    }

    T remove(ListNodePos<T> p) override {
        auto e { std::move(p->data()) };
        if (m_size == 1) {
            p->next().reset();
            m_head = nullptr;
        } else {
            p->next()->prev() = p->prev();
            if (p == m_head) {
                m_head = p->next();
            }
            p->prev()->next() = std::move(p->next());
        }
        --m_size;
        return e;
    }
};

}