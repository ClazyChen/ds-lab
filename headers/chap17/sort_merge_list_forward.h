#pragma once

// 这个文件继续讨论归并排序
// 这里讨论的是单向列表

#include "sort_framework.h"
#include "list_framework.h"

namespace clazy {

using clazy_framework::Comparator;
using clazy_framework::ListNodePos;

template <typename T, typename Container>
requires (is_base_of_v<clazy_framework::AbstractForwardList<T>, Container>)
class ForwardListMergeSort : public clazy_framework::AbstractSort<T, Container> {
protected:
    void mergeSort(Container& L, const Comparator<T>& cmp, ListNodePos<T> lo, ListNodePos<T>& hi, int size);
    void sort(Container& L, const Comparator<T>& cmp) override {
        auto pos_begin = L.begin().getPosition();
        auto pos_end = L.end().getPosition();
        mergeSort(L, cmp, pos_begin, pos_end, L.size());
    }
};

template <typename T, typename Container>
void ForwardListMergeSort<T, Container>::mergeSort(Container& L, const Comparator<T>& cmp, ListNodePos<T> lo, ListNodePos<T>& hi, int size) {
    if (size <= 1) {
        return;
    }
    auto mi = lo;                                     // 取中点
    for (int i = 0; i < size / 2; i++) {
        mi = mi->succ();
    }
    mergeSort(L, cmp, lo, mi, size / 2);              // 递归地排序左半部分
    mergeSort(L, cmp, mi, hi, size - size / 2);       // 递归地排序右半部分
    auto left = lo, right = mi;                       // 左半、右半部分的位置指针
    while (left != right && right != hi) {            // 归并
        if (cmp(L[left], L[right])) {                 // 左半部分元素小于右半部分元素
            left = left->succ();                      // 左半部分指针移动即可
        } else {                                      // 否则，需要移动右半部分元素，前插到左半部分去
            L.insertAsPred(left, L[right]);           // 进行前插
            left = left->succ();                      // 注意前插会让left指向的指针发生错误
            if (right->succ() == hi) {
                L.remove(right);                      // 需要保证hi不能被删除
                hi = right;
            } else {
                L.remove(right);                      // 删除掉已经被前插到左侧的元素，注意right会自动移动
            }
        }
    }
}

// 单向列表在操作的时候非常需要注意，前插和删除这两个操作是有风险的
// 当进行insertAsPred(left, L[right])之后，实际上是执行了一次后插操作
// 原先的情况：->left(L[left])->
// 插入之后的情况：->left(L[right])->new(L[left])->
// 这意味着两点：
// 1. 不需要像双向列表那样复位，因为原先指向最左侧的指针，以后还会继续指向最左侧
// 2. left在插入之后变成了指向新插入的元素，而不是指向最左侧的元素，所以需要进行更新
// 类似地，可以分析remove(right)的情况

}