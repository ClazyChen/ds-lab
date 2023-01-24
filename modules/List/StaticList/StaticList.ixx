module;
#include <memory>

export module List.StaticList;

export import List.StaticList.AbstractStaticList;
export import List.StaticList.StaticListNode;

import Vector;

export namespace dslab {

template <typename T>
class StaticList : public AbstractStaticList<T> {
    Vector<StaticListNode<T>> m_data {};
    static constexpr Rank NPOS { 0 };
    static constexpr Rank HEAD { 1 };
    static constexpr Rank TAIL { 2 };
    static constexpr Rank META_SIZE { 3 };

    StaticListNode<T>& getNode(Rank r) override {
        return m_data[r];
    }

public:
    Rank head() const override { return HEAD; }
    Rank tail() const override { return TAIL; }
    size_t size() const override {
        return m_data.size() - META_SIZE;
    }

    StaticList() { clear(); }
    StaticList(const StaticList& list) : m_data(list.m_data) {}
    StaticList(StaticList&& list) : m_data(std::move(list.m_data)) {
        list.clear();
    }
    StaticList& operator=(const StaticList& list) {
        if (this != &list) {
            m_data = list.m_data;
        }
        return *this;
    }
    StaticList& operator=(StaticList&& list) {
        if (this != &list) {
            m_data = std::move(list.m_data);
            list.clear();
        }
        return *this;
    }
    StaticList(std::initializer_list<T> list) : StaticList() {
        for (auto& item : list) {
            push_back(item);
        }
    }
    StaticList& operator=(std::initializer_list<T> list) {
        StaticList tmp { list };
        *this = std::move(tmp);
        return *this;
    }
    virtual ~StaticList() = default;

    void clear() {
        m_data.resize(META_SIZE);
        getNode(HEAD).next() = TAIL;
        getNode(TAIL).prev() = HEAD;
    }

    Rank insertAsNext(Rank p, const T& e) override {
        Rank r { m_data.size() };
        m_data.push_back(StaticListNode<T>(e, p, getNode(p).next()));
        getNode(p).next() = r;
        getNode(getNode(r).next()).prev() = r;
        return r;
    }

    Rank insertAsPrev(Rank p, const T& e) override {
        Rank r { m_data.size() };
        m_data.push_back(StaticListNode<T>(e, getNode(p).prev(), p));
        getNode(p).prev() = r;
        getNode(getNode(r).prev()).next() = r;
        return r;
    }

    Rank insertAsNext(Rank p, T&& e) override {
        Rank r { m_data.size() };
        m_data.push_back(StaticListNode<T>(std::move(e), p, getNode(p).next()));
        getNode(p).next() = r;
        getNode(getNode(r).next()).prev() = r;
        return r;
    }

    Rank insertAsPrev(Rank p, T&& e) override {
        Rank r { m_data.size() };
        m_data.push_back(StaticListNode<T>(std::move(e), getNode(p).prev(), p));
        getNode(p).prev() = r;
        getNode(getNode(r).prev()).next() = r;
        return r;
    }

    Rank find(const T& e) override {
        Rank r { getNode(HEAD).next() };
        while (r != TAIL) {
            if (getNode(r).data() == e) {
                return r;
            }
            r = getNode(r).next();
        }
        return TAIL;
    }

    T remove(Rank r) override {
        T e { std::move(getNode(r).data()) };
        getNode(getNode(r).prev()).next() = getNode(r).next();
        getNode(getNode(r).next()).prev() = getNode(r).prev();
        Rank s { m_data.size() - 1 };
        if (r != s) {
            getNode(r).data() = std::move(getNode(s).data());
            getNode(getNode(s).prev()).next() = r;
            getNode(getNode(s).next()).prev() = r;
        }
        m_data.pop_back();
        return e;
    }
};


}