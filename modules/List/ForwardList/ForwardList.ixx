module;
#include <memory>

export module ForwardList;

export import ForwardList.AbstractForwardList;
export import ForwardList.ForwardListNode;

export namespace dslab {

template <typename T>
class ForwardList : public AbstractForwardList<T> {
    ForwardListNodeInst<T> m_head { nullptr };
    ForwardListNodePos<T> m_tail { nullptr };
    size_t m_size { 0 };

    void initialize() {
        m_head.create();
        m_head->next().create();
        m_tail = m_head->next();
        m_size = 0;
    }

public:
    ForwardListNodePos<T> head() override { return m_head; }
    ForwardListNodeConstPos<T> head() const override { return m_head; }
    ForwardListNodePos<T> tail() override { return m_tail; }
    ForwardListNodeConstPos<T> tail() const override { return m_tail; }
    size_t size() const override { return m_size; }

    ForwardList() { initialize(); }
    ForwardList(const ForwardList& list) : ForwardList() {
        auto p { head() };
        for (auto&& item : list) {
            p = this->insertAsNext(p, item);
        }
    }
    ForwardList(ForwardList&& list) noexcept {
        m_head = std::move(list.m_head);
        m_tail = list.m_tail;
        m_size = list.m_size;
        list.initialize();
    }
    ForwardList& operator=(const ForwardList& list) {
        if (this != &list) {
            ForwardList tmp { list };
            std::swap(m_head, tmp.m_head);
            std::swap(m_tail, tmp.m_tail);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }
    ForwardList& operator=(ForwardList&& list) noexcept {
        if (this != &list) {
            ForwardList tmp { std::move(list) };
            std::swap(m_head, tmp.m_head);
            std::swap(m_tail, tmp.m_tail);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }

    ForwardList(std::initializer_list<T> ilist) : ForwardList() {
        auto p { head() };
        for (auto&& item : ilist) {
            p = this->insertAsNext(p, item);
        }
    }

    ForwardList& operator=(std::initializer_list<T> ilist) {
        ForwardList tmp { ilist };
        std::swap(m_head, tmp.m_head);
        std::swap(m_tail, tmp.m_tail);
        std::swap(m_size, tmp.m_size);
        return *this;
    }

    virtual ~ForwardList() = default;

    void clear() override {
        m_head->next().create();
        m_tail = m_head->next();
        m_size = 0;
    }

    ForwardListNodePos<T> insertAsNext(ForwardListNodePos<T> p, const T& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(e) };
        node->next() = std::move(p->next());
        p->next() = std::move(node);
        ++m_size;
        return p->next();
    }

    ForwardListNodePos<T> insertAsNext(ForwardListNodePos<T> p, T&& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(std::move(e)) };
        node->next() = std::move(p->next());
        p->next() = std::move(node);
        ++m_size;
        return p->next();
    }

    ForwardListNodePos<T> insertAsPrev(ForwardListNodePos<T> p, const T& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(std::move(p->data())) };
        node->next() = std::move(p->next());
        p->next() = std::move(node);
        p->data() = e;
        if (m_tail == p) {
            m_tail = p->next();
        }
        ++m_size;
        return p;
    }

    ForwardListNodePos<T> insertAsPrev(ForwardListNodePos<T> p, T&& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(std::move(p->data())) };
        node->next() = std::move(p->next());
        p->next() = std::move(node);
        p->data() = std::move(e);
        if (m_tail == p) {
            m_tail = p->next();
        }
        ++m_size;
        return p;
    }

    ForwardListNodePos<T> find(const T& e) const override {
        ForwardListNodePos<T> p { m_head->next() };
        while (p != nullptr) {
            if (p->data() == e) {
                return p;
            }
            p = p->next();
        }
        return m_tail;
    }

    T remove(ForwardListNodePos<T> p) override {
        auto e { std::move(p->data()) };
        p->data() = std::move(p->next()->data());
        p->next() = std::move(p->next()->next());
        if (p->next() == nullptr) {
            m_tail = p;
        }
        --m_size;
        return e;
    }
};

}