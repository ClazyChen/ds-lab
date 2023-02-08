module;
#include <memory>

export module ForwardList;

export import ForwardList.AbstractForwardList;
export import ForwardList.ForwardListNode;

export namespace dslab {

template <typename T>
class ForwardList : public AbstractForwardList<T> {
    std::unique_ptr<ForwardListNode<T>> m_head { nullptr };
    ForwardListNode<T>* m_tail { nullptr };
    size_t m_size { 0 };

    void initialize() {
        m_head = std::make_unique<ForwardListNode<T>>();
        m_head->next() = std::make_unique<ForwardListNode<T>>();
        m_tail = m_head->next().get();
        m_size = 0;
    }

public:
    ForwardListNode<T>* head() override { return m_head.get(); }
    const ForwardListNode<T>* head() const override { return m_head.get(); }
    ForwardListNode<T>* tail() override { return m_tail; }
    const ForwardListNode<T>* tail() const override { return m_tail; }
    size_t size() const override { return m_size; }

    ForwardList() { initialize(); }
    ForwardList(const ForwardList& list) : ForwardList() {
        auto p { head() };
        for (auto&& item : list) {
            p = this->insertAsNext(p, item);
        }
    }
    ForwardList(ForwardList&& list) {
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
    ForwardList& operator=(ForwardList&& list) {
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
        m_head->next() = std::make_unique<ForwardListNode<T>>();
        m_tail = m_head->next().get();
        m_size = 0;
    }

    ForwardListNode<T>* insertAsNext(ForwardListNode<T>* p, const T& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(e) };
        node->next() = std::move(p->next());
        p->next() = std::move(node);
        ++m_size;
        return p->next().get();
    }

    ForwardListNode<T>* insertAsNext(ForwardListNode<T>* p, T&& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(std::move(e)) };
        node->next() = std::move(p->next());
        p->next() = std::move(node);
        ++m_size;
        return p->next().get();
    }

    ForwardListNode<T>* insertAsPrev(ForwardListNode<T>* p, const T& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(std::move(p->data())) };
        node->next() = std::move(p->next());
        p->next() = std::move(node);
        p->data() = e;
        if (m_tail == p) {
            m_tail = p->next().get();
        }
        ++m_size;
        return p;
    }

    ForwardListNode<T>* insertAsPrev(ForwardListNode<T>* p, T&& e) override {
        auto node { std::make_unique<ForwardListNode<T>>(std::move(p->data())) };
        node->next() = std::move(p->next());
        p->next() = std::move(node);
        p->data() = std::move(e);
        if (m_tail == p) {
            m_tail = p->next().get();
        }
        ++m_size;
        return p;
    }

    ForwardListNode<T>* find(const T& e) const override {
        auto p { m_head->next().get() };
        while (p != nullptr) {
            if (p->data() == e) {
                return p;
            }
            p = p->next().get();
        }
        return m_tail;
    }

    T remove(ForwardListNode<T>* p) override {
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