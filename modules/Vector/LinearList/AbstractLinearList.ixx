module;
#include <utility>

export module LinearList.AbstractLinearList;

import Framework.DataStructure;

export namespace dslab {

// 抽象线性表
// T表示元素类型，Pos表示元素的位置类型
template <typename T, typename Pos>
class AbstractLinearList : public DataStructure<T> {
public:
    virtual T& get(Pos p) = 0; // 取出位置p处的元素
    virtual void set(Pos p, const T& e) = 0; // 将位置p处的元素设置为e
    virtual void set(Pos p, T&& e) = 0; // 将位置p处的元素设置为e

    virtual Pos insert(Pos p, const T& e) = 0; // 在位置p处插入元素e
    virtual Pos insert(Pos p, T&& e) = 0; // 在位置p处插入元素e
    virtual Pos find(const T& e) const = 0; // 查找元素e
    virtual T remove(Pos p) = 0; // 删除位置p处的元素
    virtual void clear() = 0; // 清空线性表

    virtual Pos first() const = 0; // 返回线性表的第一个元素的位置
    virtual Pos last() const = 0; // 返回线性表的最后一个元素的位置
    virtual Pos next(Pos p) const = 0; // 返回位置p的后继位置
    virtual Pos prev(Pos p) const = 0; // 返回位置p的前驱位置
    virtual bool end(Pos p) const = 0; // 判断位置p是否是表尾

    virtual T& operator[](Pos p) {
        return get(p);
    }
    virtual T& front() {
        return get(first());
    }
    virtual T& back() {
        return get(last());
    }

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using position_type = Pos;

    virtual const T& get(Pos p) const {
        return const_cast<AbstractLinearList*>(this)->get(p);
    }
    virtual const T& operator[](Pos p) const {
        return get(p);
    }
    virtual const T& front() const {
        return get(first());
    }
    virtual const T& back() const {
        return get(last());
    }

    virtual void push_back(const T& e) {
        insert(next(last()), e);
    }
    virtual void push_back(T&& e) {
        insert(next(last()), std::move(e));
    }
    virtual void push_front(const T& e) {
        insert(first(), e);
    }
    virtual void push_front(T&& e) {
        insert(first(), std::move(e));
    }
    virtual T pop_back() {
        return remove(last());
    }
    virtual T pop_front() {
        return remove(first());
    }
};

}