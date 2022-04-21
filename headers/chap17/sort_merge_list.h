#pragma once

// 这个文件讨论归并排序
// 这里讨论的是双向列表
// 需要注意的是，双向列表和单向列表的排序算法是不同的
// 这是因为单向列表无法查找前驱节点，并且移动节点的时候不安全

#include "sort_framework.h"
#include "list_framework.h"

namespace clazy {

using clazy_framework::Comparator;
using clazy_framework::ListNodePos;

template <typename T, typename Container>
requires (is_base_of_v<clazy_framework::AbstractBidirectionalList<T>, Container>)
class ListMergeSort : public clazy_framework::AbstractSort<T, Container> {
protected:
    void mergeSort(Container& L, const Comparator<T>& cmp, ListNodePos<T>& lo, ListNodePos<T> hi, int size);
    void sort(Container& L, const Comparator<T>& cmp) override {
        auto pos_begin = begin(L).getPosition();
        auto pos_end = end(L).getPosition();
        mergeSort(L, cmp, pos_begin, pos_end, L.size());
    }
};

// 归并排序的双向列表版本
template <typename T, typename Container>
void ListMergeSort<T, Container>::mergeSort(Container& L, const Comparator<T>& cmp, ListNodePos<T>& lo, ListNodePos<T> hi, int size) {
    if (size <= 1) {
        return;                                       // 递归边界：只有0或1个元素
    }
    auto mi = lo;                                     // 取中点
    for (int i = 0; i < size / 2; i++) {
        mi = mi->succ();
    }
    mergeSort(L, cmp, lo, mi, size / 2);              // 递归地排序左半部分
    mergeSort(L, cmp, mi, hi, size - size / 2);       // 递归地排序右半部分
    auto left = lo, right = mi;                       // 左半、右半部分的位置指针
    auto lo_pred = lo->pred();                        // 双向列表在最后需要将lo复位
    while (left != right && right != hi) {            // 归并
        if (cmp(L[left], L[right])) {                 // 左半部分元素小于右半部分元素
            left = left->succ();                      // 左半部分指针移动即可
        } else {                                      // 否则，需要移动右半部分元素，前插到左半部分去
            L.insertAsPred(left, L[right]);           // 进行前插
            right = right->succ();                    // 右半部分指针移动
            L.remove(right->pred());                  // 删除掉已经被前插到左侧的元素
        }
    }
    lo = lo_pred->succ();                             // 将lo复位
}

}