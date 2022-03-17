#pragma once

#include "list.h"
#include "search.h"

namespace clazy {

using clazy_framework::SearchResult;
using clazy_framework::Comparator;

// 列表的查找
// Pos在成功时，表示查找到的元素的位置
// 在失败时，指示应当被前插的位置
template <typename T, typename Container = List<T>>
requires (is_base_of_v<clazy_framework::AbstractList<T>, Container>)
using ListSearch = clazy_framework::OrderedSearch<T, Container, ListNodePos<T>>;

// 顺序查找
// 在查找这个环节里，单向链表和双向链表可以达成统一
template <typename T, typename Node = ListNode<T>, typename Container = List<T, Node>>
requires (is_base_of_v<clazy_framework::AbstractList<T>, Container> && clazy_framework::ListNodeType<T, Node>)
class ListSequentialSearch : public ListSearch<T, Container> {
protected:
    virtual SearchResult<ListNodePos<T>> search(Container& L, const T& e, const Comparator<T>& cmp) {
        ListNodePos<T> cur = begin(L).base(), last = nullptr;
        while (cur != end(L).base() && cmp(cur->data(), e)) {
            last = cur;
            cur = cur->succ();            
        } // 找到cur：第一个大于e的元素
        if (last != nullptr && last->data() == e) { // 如果last恰好等于e
            return {true, last}; // 那么last就是最后一个等于e的元素
        } else {
            return {false, cur}; // 否则，查找失败，应当对cur做前插
        }
    }
};

}