#include "list_sort.h"
#include "vector_sort.h"
#include "random.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子将会讨论列表的排序和查找
// 同样，我们拿单向链表、双向链表和向量来对比
template <typename T>
using ForwardList = clazy::List<T, clazy::ForwardListNode<T>>;

template <typename T>
using BidirectionalList = clazy::List<T>;

template <typename T>
using Vector = clazy::Vector<T>;

// 各自的排序算法
template <typename T>
using ForwardListMergeSort = clazy::ListMergeSort<T, clazy::ForwardListNode<T>, ForwardList<T>>;

template <typename T>
using BidirectionalListMergeSort = clazy::ListMergeSort<T, clazy::ListNode<T>, BidirectionalList<T>>;

template <typename T>
using VectorMergeSort = clazy::VectorMergeSort<T, Vector<T>>;

// 排序问题

int main() {
    ForwardList<int> L;
    ForwardListMergeSort<int> s;
    for (int i = 0; i < 10; i++) {
        L.push_back(Random::nextIntBetween(0, 20));
    }
    cout << L << endl;
    s.apply(L);
    cout << L << endl;
    return 0;
}