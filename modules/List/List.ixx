export module List;

export import List.AbstractList;
export import List.ListNode;
import std;

export namespace dslab {

template <typename T>
class List : public AbstractList<T> {
    ListNodeInst<T> m_head { nullptr };
    ListNodePos<T> m_tail { nullptr };
    size_t m_size { 0 };

    void initialize() {
        m_head.create();
        m_head->next().create();
        m_tail = m_head->next();
        m_tail->prev() = m_head;
        m_size = 0;
    }

public:
    ListNodePos<T> head() override { return m_head; }
    ListNodeConstPos<T> head() const override { return m_head; }
    ListNodePos<T> tail() override { return m_tail; }
    ListNodeConstPos<T> tail() const override { return m_tail; }
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
        m_head->next().create();
        m_tail = m_head->next();
        m_tail->prev() = m_head;
        m_size = 0;
    }

    ListNodePos<T> insertAsNext(ListNodePos<T> p, const T& e) override {
        auto node { ListNodeInst<T>::make(e) };
        node->next() = std::move(p->next());
        node->next()->prev() = node;
        node->prev() = p;
        p->next() = std::move(node);
        ++m_size;
        return p->next();
    }

    ListNodePos<T> insertAsPrev(ListNodePos<T> p, const T& e) override {
        auto q { p->prev() };
        return insertAsNext(q, e);
    }

    ListNodePos<T> insertAsNext(ListNodePos<T> p, T&& e) override {
        auto node { ListNodeInst<T>::make(std::move(e)) };
        node->next() = std::move(p->next());
        node->next()->prev() = node;
        node->prev() = p;
        p->next() = std::move(node);
        ++m_size;
        return p->next();
    }

    ListNodePos<T> insertAsPrev(ListNodePos<T> p, T&& e) override {
        auto q { p->prev() };
        return insertAsNext(q, std::move(e));
    }

    ListNodePos<T> find(const T& e) const override {
        ListNodePos<T> p { m_head->next() };
        while (p != m_tail) {
            if (p->data() == e) {
                return p;
            }
            p = p->next();
        }
        return m_tail;
    }

    T remove(ListNodePos<T> p) override {
        auto e { std::move(p->data()) };
        p->next()->prev() = p->prev();
        p->prev()->next() = std::move(p->next());
        --m_size;
        return e;
    }

    std::string type_name() const override {
        return "Bidirectional List";
    }
};

}