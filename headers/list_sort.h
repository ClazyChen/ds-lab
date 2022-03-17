#pragma once
#include "list.h"
#include "sort.h"

namespace clazy {

using clazy_framework::Comparator;

// 列表的归并排序
template <typename T, typename Node = ListNode<T>, typename Container = List<T, Node>>
requires (is_base_of_v<clazy_framework::AbstractList<T>, Container> && clazy_framework::ListNodeType<T, Node>)
class ListMergeSort : public clazy_framework::Sort<T, Container> {
protected:
    virtual void mergeSort(ListIterator<T>& it_begin, ListIterator<T> it_end, int size, const Comparator<T>& cmp);
    virtual void sort(Container& L, const Comparator<T>& cmp) {
        auto it_begin = begin(L);
        mergeSort(it_begin, end(L), L.size(), cmp);
    }
};

// 归并排序（使用迭代器设计）
// 单向链表和双向链表混合使用
template <typename T, typename Node, typename Container>
void ListMergeSort<T, Node, Container>::mergeSort(ListIterator<T>& it_begin, ListIterator<T> it_end, int size, const Comparator<T>& cmp) {
    if (size <= 1) {                                  // List的迭代器不能做减法
        return;                                       // 递归边界：只有0或1个元素
    }
    auto it_mid = it_begin + size / 2;                // 中点
    mergeSort(it_begin, it_mid, size / 2, cmp);       // 递归地排序左半部分
    mergeSort(it_mid, it_end, size - size / 2, cmp);  // 递归地排序右半部分
    auto it_left = it_begin, it_right = it_mid;       // 开始归并
    auto it_bpred = it_begin;                         // 如果是双向链表需要用到这个，来进行begin的复位
    auto it_mpred = it_begin;                         // 如果是单向链表需要用到这个，来界定左半部分的边界
    if constexpr (Node::isBidirectional()) {          // 这是因为归并的过程中直接前插，有可能有节点插入到begin之前
        it_bpred--;                                   // 不进行复位的话，则归并结束之后begin指向的节点不再是实际的链表首节点
    } else {
        it_mpred += size / 2 - 1;                     // 这样摘出右边的节点，就知道它的前驱的位置
    }
    while (it_left != it_right && it_right != it_end) { // 如果两段都没有被归并完
        if (cmp(*it_left, *it_right)) {               // 比较还没有被归并的最小元素
            it_left++;                                // 如果左半部分小，则不进行移动操作
        } else {                                      // 如果右半部分小，将它前插到左半部分去
            auto it_move = it_right++;                // 需要被摘除的节点位置
            if constexpr (Node::isBidirectional()) {  // 如果是双向链表，直接删除+前插
                it_move->pred()->setSucc(it_right->setPred(it_move->pred())); // 将要移动的节点从原位置摘出
                it_move->setPred(it_left->pred()->setSucc(it_move.base())); // 将it_move插入到it_left前面
                it_left->setPred(it_move->setSucc(it_left.base()));    // 在双向链表的场合，直接前插，也不用让left后移
            } else {                                  // 如果是单向链表，用后删代替删除，用后插代替前插
                it_mpred->setSucc(it_right.base());   // 将要移动的节点从原位置摘出
                it_move->setSucc(it_left->succ());    // 将it_move插入到it_left后面
                it_left->setSucc(it_move.base());
                swap(*it_left, *it_move);             // 交换两个节点的值，从而实现等价前插
                if (it_mpred == it_left) {            // 如果需要扩展左半部分的边界
                    it_mpred = it_move;
                }
                it_left = it_move;                    // 左半部分的指针需要向后移
            }
        }
    }
    if constexpr (Node::isBidirectional()) {          // 双向链表需要的begin复位
        it_begin = it_bpred + 1;
    }
}

}