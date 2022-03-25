#pragma once

#include "vector.h"
#include "search.h"

namespace clazy {

using clazy_framework::SearchResult;
using clazy_framework::Comparator;

// 向量的查找
// Rank在成功时，表示查找到的元素的秩；在失败时，指示应当被插入的位置
template <typename T, typename Container = Vector<T>>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
using VectorSearch = clazy_framework::OrderedSearch<T, Rank, Container>;

// 顺序查找
template <typename T, typename Container = Vector<T>>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class VectorSequentialSearch : public VectorSearch<T, Container> {
protected:
    virtual SearchResult<Rank> search(Container& V, const T& e, const Comparator<T>& cmp) override {
        for (Rank r : views::iota(0, V.size()) | views::reverse) {
            if (cmp(V[r], e)) {
                if (V[r] == e) {
                    return {true, r};
                } else {
                    return {false, r+1};
                }
            }
        }
        return {false, 0};
    }
};

// 折半查找（迭代方法）
template <typename T, typename Container = Vector<T>>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class VectorBinarySearch : public VectorSearch<T, Container> {
protected:
    virtual VectorIterator<T> binarySearch(VectorIterator<T> it_begin, VectorIterator<T> it_end, const T& e, const Comparator<T>& cmp);
    virtual SearchResult<Rank> search(Container& V, const T& e, const Comparator<T>& cmp) override {
        auto pos = binarySearch(begin(V), end(V), e, cmp);
        if (pos > begin(V) && *(pos - 1) == e) {
            return {true, pos - begin(V) - 1};
        } else {
            return {false, pos - begin(V)};
        }
    }
};

// 迭代方法（对应邓俊辉《数据结构》形式C）
template <typename T, typename Container>
VectorIterator<T> VectorBinarySearch<T, Container>::binarySearch(VectorIterator<T> it_begin, VectorIterator<T> it_end, const T& e, const Comparator<T>& cmp) {
    while (it_begin < it_end) {        // 保证it_begin左侧的元素不小于e，it_end及右侧的元素大于e
        auto it_mid = it_begin + (it_end - it_begin) / 2; // 取中点
        if (cmp(*it_mid, e)) {         // 如果e不小于中点 
            it_begin = it_mid + 1;     // 则进入右半部分查找，注意需要+1避免无限循环
        } else {                       // 如果e小于中点
            it_end = it_mid;           // 则进入左半部分查找
        }
    }
    return it_begin;                   // 如果查找成功，则it_begin-1是秩最大的等于e的元素；如果查找失败，it_begin是e的插入位置
}

}
