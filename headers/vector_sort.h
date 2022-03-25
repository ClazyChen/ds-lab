#pragma once
#include "vector.h"
#include "sort.h"

namespace clazy {

using clazy_framework::Comparator;

// 向量的排序
// Rank在成功时，表示查找到的元素的秩；在失败时，指示应当被插入的位置
template <typename T, typename Container = Vector<T>>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
using VectorSort = clazy_framework::Sort<T, Container>;

// 向量的归并排序
template <typename T, typename Container = Vector<T>>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class VectorMergeSort : public VectorSort<T, Container> {
private:
    Container W;
protected:
    virtual void mergeSort(VectorIterator<T> it_begin, VectorIterator<T> it_end, const Comparator<T>& cmp);
    virtual void sort(Container& V, const Comparator<T>& cmp) override {
        mergeSort(begin(V), end(V), cmp);
    }
};

// 标准归并排序（使用迭代器设计）
template <typename T, typename Container>
void VectorMergeSort<T, Container>::mergeSort(VectorIterator<T> it_begin, VectorIterator<T> it_end, const Comparator<T>& cmp) {
    if (it_end - it_begin <= 1) {
        return;                                       // 递归边界：只有0或1个元素
    }
    auto it_mid = it_begin + (it_end - it_begin) / 2; // 取中点
    mergeSort(it_begin, it_mid, cmp);                 // 递归地排序左半部分
    mergeSort(it_mid, it_end, cmp);                   // 递归地排序右半部分
    W.resize(it_mid - it_begin);
    copy(it_begin, it_mid, begin(W));                 // 开始归并，前半部分复制到辅助数组，后半部分不用复制
    auto it_left = begin(W), it_right = it_mid;       // 左半部分和右半部分的迭代器
    auto it_merge = it_begin;                         // 归并的迭代器
    while (it_left < end(W) && it_right < it_end) {   // 两个部分的元素都没有用尽时
        if (cmp(*it_left, *it_right)) {               // 比较它们还没有被归并的最小元素
            *(it_merge++) = *(it_left++);             // 将二者的较小者归并
        } else {
            *(it_merge++) = *(it_right++);
        }
    }
    copy(it_left, end(W), it_merge);                  // 辅助数组中的剩余元素归并进去（后半部分不用显式归并）
}


}