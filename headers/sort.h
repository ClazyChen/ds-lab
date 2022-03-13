#pragma once

#include "vector.h"

namespace clazy_framework {

// 这个文件考虑一个基本问题：排序
template <typename T, typename Container>
requires (is_base_of_v<AbstractLinearStructure<T>, Container>)
class Sort : public Algorithm {
protected:
    virtual void sort(Container& C, const Comparator<T>& cmp) = 0;
public:
    virtual void apply(Container& C, const Comparator<T>& cmp = less_equal<T>()) {
        sort(C, cmp);
    }
};

}

namespace clazy {

// 向量的归并排序
template <typename T>
class VectorMergeSort : public clazy_framework::Sort<T, Vector<T>> {
private:
    Vector<T> W;
protected:
    virtual void mergeSort(VectorIterator<T> it_begin, VectorIterator<T> it_end, const function<bool(const T&, const T&)>& cmp);
    virtual void sort(Vector<T>& V, const function<bool(const T&, const T&)>& cmp) {
        mergeSort(begin(V), end(V), cmp);
    }
};

// 标准归并排序（使用迭代器设计）
template <typename T>
void VectorMergeSort<T>::mergeSort(VectorIterator<T> it_begin, VectorIterator<T> it_end, const function<bool(const T&, const T&)>& cmp) {
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
