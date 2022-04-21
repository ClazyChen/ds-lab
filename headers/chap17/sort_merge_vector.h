#pragma once

// 这个文件讨论归并排序
// 值得注意的是，归并排序在向量和列表上的实现方式是显著不同的
// 这主要来自于向量需要开辟额外的内存空间，而列表不需要

#include "sort_framework.h"
#include "vector_framework.h"

namespace clazy {

using clazy_framework::Comparator;
using clazy_framework::Rank;

template <typename T, typename Container>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class VectorMergeSort : public clazy_framework::AbstractSort<T, Container> {
private:
    Container W;
protected:
    void mergeSort(Container& V, const Comparator<T>& cmp, Rank lo, Rank hi);
    void sort(Container& V, const Comparator<T>& cmp) override {
        mergeSort(V, cmp, 0, V.size());
    }
public:
    void clear() override {
        W.clear();
    }
};

template <typename T, typename Container>
void VectorMergeSort<T, Container>::mergeSort(Container& V, const Comparator<T>& cmp, Rank lo, Rank hi) {
    if (hi - lo <= 1) {
        return;                                       // 递归边界：只有0或1个元素
    }
    Rank mi = (lo + hi) / 2;                          // 取中点
    mergeSort(V, cmp, lo, mi);                        // 递归地排序左半部分
    mergeSort(V, cmp, mi, hi);                        // 递归地排序右半部分
    W.resize(mi - lo);
    copy(V.data() + lo, V.data() + mi, W.data());     // 开始归并，前半部分复制到辅助数组，后半部分不用复制
    Rank left = 0, right = mi, merge = lo;            // 左半、右半部分的索引，归并的索引 
    while (left < W.size() && right < hi) {           // 归并
        if (cmp(W[left], V[right])) {
            V[merge++] = W[left++];
        } else {
            V[merge++] = V[right++];
        }
    }
    copy(W.data() + left, W.data() + W.size(), V.data() + merge); // 剩余部分复制回原数组
}

}