#include "list_sort.h"
#include "vector_sort.h"
#include "list_search.h"
#include "vector_search.h"
#include "list.h"
#include "list_forward.h"
#include "vector.h"
#include "random.h"
using namespace clazy_framework;

// 这个例子将会讨论列表的排序
// 同样，我们拿单向链表、双向链表和作为参考对象的向量来对比
template <typename T>
using ForwardList = clazy::ForwardList<T>;

template <typename T>
using BidirectionalList = clazy::List<T>;

template <typename T>
using Vector = clazy::Vector<T>;

// 各自的查找算法，其中向量采用折半查找
template <typename T>
using ForwardListSearch = clazy::SequentialSearch<T, ForwardList<T>>;

template <typename T>
using BidirectionalListSearch = clazy::SequentialSearch<T, BidirectionalList<T>>;

template <typename T>
using VectorSearch = clazy::BinarySearch<T, Vector<T>>;
