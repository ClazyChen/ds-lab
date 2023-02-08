module;
#include <memory>

export module List;

export import List.AbstractList;
export import List.ListNode;

export namespace dslab {

template <typename T>
class List : public AbstractList<T> {
    std::unique_ptr<ListNode<T>> m_head { nullptr };
    ListNode<T>* m_tail { nullptr };
    size_t m_size { 0 };

    void initialize() {
        m_head = std::make_unique<ListNode<T>>();
        m_head->next() = std::make_unique<ListNode<T>>();
        m_tail = m_head->next().get();
        m_tail->prev() = m_head.get();
        m_size = 0;
    }

public:
    ListNode<T>* head() override { return m_head.get(); }
    const ListNode<T>* head() const override { return m_head.get(); }
    ListNode<T>* tail() override { return m_tail; }
    const ListNode<T>* tail() const override { return m_tail; }
    size_t size() const override { return m_size; }

    List() { initialize(); }
    List(const List& list) : List() {
        auto p { head() };
        for (auto&& item : list) {
            p = this->insertAsNext(p, item);
        }
    }
    List(List&& list) noexcept {
        m_head = std::move(list.m_head);
        m_tail = list.m_tail;
        m_size = list.m_size;
        list.initialize();
    }
    List& operator=(const List& list) {
        if (this != &list) {
            List tmp { list };
            std::swap(m_head, tmp.m_head);
            std::swap(m_tail, tmp.m_tail);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    List& operator=(List&& list) noexcept {
        if (this != &list) {
            List tmp { std::move(list) };
            std::swap(m_head, tmp.m_head);
            std::swap(m_tail, tmp.m_tail);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }

    List(std::initializer_list<T> ilist) : List() {
        auto p { head() };
        for (auto&& item : ilist) {
            p = this->insertAsNext(p, item);
        }
    }

    List& operator=(std::initializer_list<T> ilist) {
        List tmp { ilist };
        std::swap(m_head, tmp.m_head);
        std::swap(m_tail, tmp.m_tail);
        std::swap(m_size, tmp.m_size);
        return *this;
    }

    virtual ~List() = default;

    void clear() override {
        m_head->next() = std::make_unique<ListNode<T>>();
        m_tail = m_head->next().get();
        m_tail->prev() = m_head.get();
        m_size = 0;
    }

    ListNode<T>* insertAsNext(ListNode<T>* p, const T& e) override {
        auto node { std::make_unique<ListNode<T>>(e) };
        node->next() = std::move(p->next());
        node->next()->prev() = node.get();
        node->prev() = p;
        p->next() = std::move(node);
        ++m_size;
        return p->next().get();
    }

    ListNode<T>* insertAsPrev(ListNode<T>* p, const T& e) override {
        auto q { p->prev() };
        return insertAsNext(q, e);
    }

    ListNode<T>* insertAsNext(ListNode<T>* p, T&& e) override {
        auto node { std::make_unique<ListNode<T>>(std::move(e)) };
        node->next() = std::move(p->next());
        node->next()->prev() = node.get();
        node->prev() = p;
        p->next() = std::move(node);
        ++m_size;
        return p->next().get();
    }

    ListNode<T>* insertAsPrev(ListNode<T>* p, T&& e) override {
        auto q { p->prev() };
        return insertAsNext(q, std::move(e));
    }

    ListNode<T>* find(const T& e) const override {
        auto p { m_head->next().get() };
        while (p != m_tail) {
            if (p->data() == e) {
                return p;
            }
            p = p->next().get();
        }
        return m_tail;
    }

    T remove(ListNode<T>* p) override {
        auto e { std::move(p->data()) };
        p->next()->prev() = p->prev();
        p->prev()->next() = std::move(p->next());
        --m_size;
        return e;
    }
};

}