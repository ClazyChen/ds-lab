export module List.StaticList.AbstractStaticList;

import LinearList;
import List.StaticList.StaticListIterator;
import List.StaticList.StaticListNode;

import std;

using Rank = size_t;

export namespace dslab {

template <typename T>
class AbstractStaticList : public AbstractLinearList<T, Rank> {
protected:
    virtual StaticListNode<T>& getNode(Rank r) = 0;
    virtual const StaticListNode<T>& getNode(Rank r) const = 0;
public:
    virtual Rank head() const = 0; // 返回首哨兵节点
    virtual Rank tail() const = 0; // 返回尾哨兵节点
    virtual size_t size() const = 0; // 返回列表的规模
    virtual Rank insertAsNext(Rank p, const T& e) = 0; // 将e当作p的后继插入
    virtual Rank insertAsNext(Rank p, T&& e) = 0; // 将e当作p的后继插入
    virtual Rank insertAsPrev(Rank p, const T& e) = 0; // 将e当作p的前驱插入
    virtual Rank insertAsPrev(Rank p, T&& e) = 0; // 将e当作p的前驱插入
    virtual Rank find(const T& e) const = 0; // 查找元素e
    virtual T remove(Rank p) = 0; // 删除位置p处的元素
    virtual void clear() = 0; // 清空列表

    Rank insert(Rank p, const T& e) override {
        return insertAsPrev(p, e);
    }

    Rank insert(Rank p, T&& e) override {
        return insertAsPrev(p, std::move(e));
    }

    T& get(Rank p) override {
        return getNode(p).data();
    }

    const T& get(Rank p) const override {
        return getNode(p).data();
    }

    void set(Rank p, const T& e) override {
        getNode(p).data() = e;
    }

    void set(Rank p, T&& e) override {
        getNode(p).data() = std::move(e);
    }

    Rank first() const override {
        return next(head());
    }

    Rank last() const override {
        return prev(tail());
    }

    Rank next(Rank p) const override {
        return getNode(p).next();
    }

    Rank prev(Rank p) const override {
        return getNode(p).prev();
    }

    bool end(Rank p) const override {
        return p == tail();
    }

    void push_back(const T& e) override {
        insertAsPrev(tail(), e);
    }

    void push_back(T&& e) override {
        insertAsPrev(tail(), std::move(e));
    }

    void push_front(const T& e) override {
        insertAsNext(head(), e);
    }

    void push_front(T&& e) override {
        insertAsNext(head(), std::move(e));
    }

    using iterator = StaticListIterator<AbstractStaticList>;
    using const_iterator = ConstStaticListIterator<AbstractStaticList>;

    iterator begin() {
        return iterator { this, first() };
    }

    iterator end() {
        return iterator { this, tail() };
    }

    const_iterator begin() const {
        return const_iterator { this, first() };
    }

    const_iterator end() const {
        return const_iterator { this, tail() };
    }

    const_iterator cbegin() const {
        return const_iterator { this, first() };
    }

    const_iterator cend() const {
        return const_iterator { this, tail() };
    }

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    reverse_iterator rbegin() {
        return reverse_iterator { end() };
    }

    reverse_iterator rend() {
        return reverse_iterator { begin() };
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator { end() };
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator { begin() };
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator { cend() };
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator { cbegin() };
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractStaticList<T>& list) {
    os << "SL[";
    for (auto it { list.begin() }; it != list.end(); ++it) {
        os << *it << ", ";
    }
    if (list.size() > 0) {
        os << "\b\b";
    }
    os << "]";
    return os;
}

}
