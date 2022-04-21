#pragma once

// 这个文件给出了一个用于列表的迭代器的模板
// 实际上，列表的迭代器和线性表一样，不需要做额外的处理
// 所以这里直接采用了别名的方法

#include "linear_iterator_framework.h"
#include "list_node_framework.h"

namespace clazy_framework {

template <typename T, typename LS>
class ListIterator : public LinearIterator<T, ListNodePos<T>, LS> {
public:
    ListIterator(const LS& ls, ListNodePos<T> pos) : LinearIterator<T, ListNodePos<T>, LS>(ls, pos) {}
};

// 单向链表的迭代器有所不同，因为单向链表不支持反向的迭代
template <typename T, typename LS>
class ForwardListIterator : public LinearIterator<T, ListNodePos<T>, LS> {
public:
    using iterator_category = forward_iterator_tag;
    ForwardListIterator(const LS& ls, ListNodePos<T> pos) : LinearIterator<T, ListNodePos<T>, LS>(ls, pos) {}

    // 删除自减、减号、减号赋值
    ForwardListIterator& operator--() = delete;
    ForwardListIterator operator--(int) = delete;
    ForwardListIterator& operator-=(int offset) = delete;
    ForwardListIterator operator-(int offset) = delete;
};

}