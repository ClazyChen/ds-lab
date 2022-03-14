#pragma once
#include "list.h"
#include "sort.h"

namespace clazy {

using clazy_framework::Comparator;

// 列表的归并排序
template <typename T, typename Container = List<T>>
requires (is_base_of_v<clazy_framework::AbstractList<T>, Container>)
class ListMergeSort : public clazy_framework::Sort<T, Container> {
protected:
    virtual void mergeSort(ListIterator<T> it_begin, ListIterator<T> it_end, int size, const Comparator<T>& cmp);
    virtual void sort(Container& V, const Comparator<T>& cmp) {
        auto it_begin = begin(V);
        mergeSort(it_begin, end(V), cmp);
    }
};

// 归并排序（使用迭代器设计）
// 为了兼容单向链表，这里略微降低了性能
template <typename T, typename Container>
void ListMergeSort<T, Container>::mergeSort(ListIterator<T> it_begin, ListIterator<T> it_end, int size, const Comparator<T>& cmp) {
    if (size <= 1) {                                  // List的迭代器不能做减法
        return;                                       // 递归边界：只有0或1个元素
    }
    auto it_mpred = it_begin + (size / 2 - 1), it_mid = it_mpred + 1; // 中点，以及中点的前驱
    mergeSort(it_begin, it_mid, size, cmp);           // 递归地排序左半部分
    mergeSort(it_mid, it_end, size - size / 2, cmp);  // 递归地排序右半部分
    auto it_left = it_begin, it_right = it_mid;       // 开始归并
    while (it_left != it_right && it_right != it_end) { // 如果两段都没有被归并完
        if (cmp(*it_left, *it_right)) {               // 比较还没有被归并的最小元素
            it_left++;                                // 如果左半部分小，则不进行移动操作
        } else {                                      // 如果右半部分小，将它前插到左半部分去
            auto it_move = it_right++;                // 获取要移动的节点
            it_mpred->setSucc(it_right->setPred(it_mpred.base())); // 将要移动的节点从原位置摘出
            it_move->setSucc(it_left->succ()->setPred(it_move.base())); // 将it_move插入到it_left后面
            it_left->setSucc(it_move->setPred(it_left.base()));    // 为了兼容单向链表，统一使用后插
            swap(*it_left, *it_move);                 // 交换两个节点的值，从而实现等价前插
            it_left = it_move;                        // 左半部分的指针需要向后移
        }
    }
}


}