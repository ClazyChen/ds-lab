export module List.AbstractList;

import LinearList;
import List.ListNode;
import List.ListIterator;
import std;

export namespace dslab {

template <typename T>
class AbstractList : public AbstractLinearList<T, ListNodePos<T>> {
public:
    virtual ListNodePos<T> head() = 0; // 返回首哨兵节点
    virtual ListNodeConstPos<T> head() const = 0; // 返回首哨兵节点
    virtual ListNodePos<T> tail() = 0; // 返回尾哨兵节点
    virtual ListNodeConstPos<T> tail() const = 0; // 返回尾哨兵节点
    virtual size_t size() const = 0; // 返回列表的规模
    virtual ListNodePos<T> insertAsNext(ListNodePos<T> p, const T& e) = 0; // 将e当作p的后继插入
    virtual ListNodePos<T> insertAsNext(ListNodePos<T> p, T&& e) = 0; // 将e当作p的后继插入
    virtual ListNodePos<T> insertAsPrev(ListNodePos<T> p, const T& e) = 0; // 将e当作p的前驱插入
    virtual ListNodePos<T> insertAsPrev(ListNodePos<T> p, T&& e) = 0; // 将e当作p的前驱插入
    virtual ListNodePos<T> find(const T& e) const = 0; // 查找元素e
    virtual T remove(ListNodePos<T> p) = 0; // 删除位置p处的元素
    virtual void clear() = 0; // 清空列表

    ListNodePos<T> insert(ListNodePos<T> p, const T& e) override {
        return insertAsPrev(p, e);
    }

    ListNodePos<T> insert(ListNodePos<T> p, T&& e) override {
        return insertAsPrev(p, std::move(e));
    }

    T& get(ListNodePos<T> p) override {
        return p->data();
    }

    void set(ListNodePos<T> p, const T& e) override {
        p->data() = e;
    }

    void set(ListNodePos<T> p, T&& e) override {
        p->data() = std::move(e);
    }

    ListNodePos<T> first() const override {
        return head()->next();
    }

    ListNodePos<T> last() const override {
        return const_cast<AbstractList*>(this)->tail()->prev();
    }

    ListNodePos<T> next(ListNodePos<T> p) const override {
        return p->next();
    }

    ListNodePos<T> prev(ListNodePos<T> p) const override {
        return p->prev();
    }

    bool end(ListNodePos<T> p) const override {
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

    using iterator = ListIterator<AbstractList>;
    using const_iterator = ConstListIterator<AbstractList>;

    iterator begin() noexcept {
        return iterator { this, first() };
    }

    iterator end() noexcept {
        return iterator { this, tail() };
    }

    const_iterator begin() const noexcept {
        return const_iterator { this, first() };
    }

    const_iterator end() const noexcept {
        return const_iterator { this, const_cast<AbstractList*>(this)->tail() };
    }

    const_iterator cbegin() const noexcept {
        return const_iterator { this, first() };
    }

    const_iterator cend() const noexcept {
        return const_iterator { this, const_cast<AbstractList*>(this)->tail() };
    }

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    reverse_iterator rbegin() noexcept {
        return reverse_iterator { end() };
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator { begin() };
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator { end() };
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator { begin() };
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator { cend() };
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator { cbegin() };
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractList<T>& list) {
    os << "L[";
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