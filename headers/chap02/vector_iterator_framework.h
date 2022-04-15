#pragma once

// 这个文件给出了一个用于向量的迭代器的模板

#include "linear_iterator_framework.h"

namespace clazy_framework {

template <typename T, typename LS>
class VectorIterator : public LinearIterator<T, Rank, LS> {
public:
    using iterator_category = random_access_iterator_tag;
    
    VectorIterator(const LS& ls, Rank pos) : LinearIterator<T, Rank, LS>(ls, pos) {}

    // 需要重载加法和减法运算符，因为向量是连续存储的
    VectorIterator& operator+=(int offset) {
        this->pos += offset;
        return *this;
    }

    VectorIterator& operator-=(int offset) {
        this->pos -= offset;
        return *this;
    }

    VectorIterator operator+(int offset) {
        auto temp = *this;
        temp += offset;
        return temp;
    }

    VectorIterator operator-(int offset) {
        auto temp = *this;
        temp -= offset;
        return temp;
    }

    // 可以定义减法
    ptrdiff_t operator-(const VectorIterator& other) {
        return this->pos - other.pos;
    }

};

}