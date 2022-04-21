#pragma once

// 这个文件讨论的是向量的折半查找
// 给出了一个基本框架，用户可以通过实现binarySearch这个函数来实现自己的折半查找

#include "vector_framework.h"
#include "search_framework.h"

namespace clazy_framework {

// 折半查找的基本框架
template <typename T, typename Container>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class AbstractBinarySearch : public clazy_framework::AbstractOrderedSearch<T, Rank, Container> {
protected:
    virtual Rank binarySearch(const Container& C, const T& e, const Comparator<T>& cmp, Rank lo, Rank hi) = 0;
    SearchResult<Rank> search(const Container& C, const T& e, const Comparator<T>& cmp) override {
        Rank r = binarySearch(C, e, cmp, 0, C.size());
        if (0 <= r && r < C.size() && C[r] == e) {
            return {true, r};
        } else {
            return {false, r + 1};
        }
    }
};

}