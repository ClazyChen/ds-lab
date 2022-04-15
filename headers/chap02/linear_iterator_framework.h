#pragma once

// 这个文件给出了一个线性表的迭代器基类
// 向量和列表共享这个迭代器，因为它只需要用到线性表基类用到的几个接口
// 后面，向量和列表将具体地各自继承这个迭代器

#include "linear_framework.h"

namespace clazy_framework {

template <typename T, typename P, typename LS>
requires (is_base_of_v<AbstractLinearStructure<T, P>, LS>)
class LinearIterator : public Object {
protected:
    const LS& ls;
    P pos;
public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

    LinearIterator(const LS& ls, P pos) : ls(ls), pos(pos) {}

    bool operator==(const LinearIterator& other) const {
        return (pos == other.pos);
    }

    auto operator<=>(const LinearIterator& other) const {
        return (pos <=> other.pos);
    }

    LinearIterator& operator++() {
        pos = ls.getNext(pos);
        return *this;
    }

    LinearIterator operator++(int) {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    LinearIterator& operator--() {
        pos = ls.getPrev(pos);
        return *this;
    }

    LinearIterator operator--(int) {
        auto temp = *this;
        --(*this);
        return temp;
    }

    LinearIterator& operator+=(int offset) {
        while (offset-- > 0) {
            ++(*this);
        }
        return *this;
    }

    LinearIterator& operator-=(int offset) {
        while (offset-- > 0) {
            --(*this);
        }
        return *this;
    }

    LinearIterator operator+(int offset) {
        auto temp = *this;
        temp += offset;
        return temp;
    }

    LinearIterator operator-(int offset) {
        auto temp = *this;
        temp -= offset;
        return temp;
    }

    reference operator*() {
        return ls.get(pos);
    }

    pointer operator->() {
        return &ls.get(pos);
    }

    P getPosition() const {
        return pos;
    }

};


}