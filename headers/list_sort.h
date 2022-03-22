#pragma once
#include "list.h"
#include "sort.h"

namespace clazy {

using clazy_framework::Comparator;

// 列表的归并排序
template <typename T, typename P = ListNodePos<T>, typename Node = ListNode<T>, typename Container = DynamicList<T, Node>>
requires (is_base_of_v<clazy_framework::AbstractList<T, P, Node>, Container> && clazy_framework::is_listnode_type<T, Node>)
class ListMergeSort : public clazy_framework::Sort<T, Container> {
protected:
    virtual void mergeSort(Container& L, P& pos_begin, P pos_end, int size, const Comparator<T>& cmp);
    virtual void sort(Container& L, const Comparator<T>& cmp) {
        P pos_begin = begin(L).base();
        mergeSort(L, pos_begin, end(L).base(), L.size(), cmp);
    }
};

// 归并排序（使用迭代器设计）
// 单向链表和双向链表混合使用
template <typename T, typename P, typename Node, typename Container>
void ListMergeSort<T, P, Node, Container>::mergeSort(Container& L, P& pos_begin, P pos_end, int size, const Comparator<T>& cmp) {
    if (size <= 1) {
        return;                                       // 递归边界：只有0或1个元素
    }
    P pos_mid = pos_begin;
    for (int i : views::iota(0, size / 2)) {
        pos_mid = L.succ(pos_mid);                    // 取中点
    }
    mergeSort(L, pos_begin, pos_mid, size / 2, cmp);      // 递归地排序左半部分
    mergeSort(L, pos_mid, pos_end, size - size / 2, cmp); // 递归地排序右半部分
    P pos_left = pos_begin, pos_right = pos_mid;      // 开始归并
    P pos_bpred = pos_begin;                          // 如果是双向链表需要用到这个，来进行begin的复位
    P pos_mpred = pos_begin;                          // 如果是单向链表需要用到这个，来界定左半部分的边界
    if constexpr (Node::isBidirectional()) {          // 这是因为归并的过程中直接前插，有可能有节点插入到begin之前
        pos_bpred = L.pred(pos_begin);                // 不进行复位的话，则归并结束之后begin指向的节点不再是实际的链表首节点
    } else /* forward list */ {                       // 单向链表的情形下为了摘出右半部分指向的元素
        for (int i : views::iota(0, size / 2 - 1)) {  // 需要界定，左半部分的最后一个元素，从而保证不发生断链
            pos_mpred = L.succ(pos_mpred);            // 这里取的是mid的前驱，作为左半部分边界的初始值 
        }                                             // 这里不能放在取pos_mid的一起取，因为在递归的过程中可能会被移动
    }
    while (pos_left != pos_right && pos_right != pos_end) { // 如果两段都没有被归并完，注意左半边被归并完的特征就是left追上right
        if (cmp(L.data(pos_left), L.data(pos_right))) { // 比较还没有被归并的最小元素
            pos_left = L.succ(pos_left);              // 如果左半部分小，则不进行移动操作
        } else {                                      // 如果右半部分小，将它前插到左半部分去
            P pos_move = pos_right;                   // 准备移动到前面的元素
            pos_right = L.succ(pos_right);            // 向后移动right指针
            if constexpr (Node::isBidirectional()) {  // 如果是双向链表，直接删除+前插
                L.setSucc(L.pred(pos_move), pos_right); // 将move从右半部分摘出
                L.setPred(pos_right, L.pred(pos_move));
                L.setSucc(L.pred(pos_left), pos_move);  // 将move插入到left前面
                L.setPred(pos_move, L.pred(pos_left));
                L.setSucc(pos_move, pos_left);
                L.setPred(pos_left, pos_move);
            } else /* forward list */ {               // 如果是单向链表，用后删代替删除，用后插代替前插
                L.setSucc(pos_mpred, pos_right);      // 将move从右半部分摘出
                L.setSucc(pos_move, L.succ(pos_left)); // 将move插入到left后面
                L.setSucc(pos_left, pos_move);
                swap(L.data(pos_left), L.data(pos_move)); // 交换两个节点的值，实现等价前插
                if (pos_mpred == pos_left) {          // 可能需要扩展左半部分的边界
                    pos_mpred = pos_move;
                }
                pos_left = pos_move;                  // 左半部分的指针需要后移从而实现完整的前插
            }
        }
    }
    if constexpr (Node::isBidirectional()) {          // 双向链表需要的begin复位
        pos_begin = L.succ(pos_bpred);
    }
}

}