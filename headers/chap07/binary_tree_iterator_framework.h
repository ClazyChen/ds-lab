#pragma once

// 这个文件给出了一个二叉树迭代器的基类
// 这个基类可以派生出先序、中序、后序三种迭代器
// 使用迭代器遍历整个二叉树是O(n)的
// 但是层次遍历是不能这样迭代的，因为层次遍历破坏了二叉树固有的半线性结构，所以找后继的过程并不是分摊O(1)的

#include "binary_tree_node_framework.h"

namespace clazy_framework {

// 用来实现迭代器的算法，包括4个部分
// 1. 查找元素的后继（保证当前元素不为空）
// 2. 查找元素的前驱（保证当前元素不为空）
// 3. 迭代的第一个位置（有可能是空树）
// 4. 迭代的最后一个位置（有可能是空树）
template <typename T>
class BinaryTreeIterateProblem : public Algorithm<> {
public:
    virtual BinaryTreeNodePos<T> succ(BinaryTreeNodePos<T> pos) const = 0;
    virtual BinaryTreeNodePos<T> pred(BinaryTreeNodePos<T> pos) const = 0;
    virtual BinaryTreeNodePos<T> first(BinaryTreeNodePos<T> pos) const = 0;
    virtual BinaryTreeNodePos<T> last(BinaryTreeNodePos<T> pos) const = 0;
};

// 二叉树的迭代器，三种通用
template <typename T, typename IA>
requires (is_base_of_v<BinaryTreeIterateProblem<T>, IA>)
class BinaryTreeIterator : public Object {
private:
    BinaryTreeNodePos<T> pos;

    // 迭代算法
    static IA ia;

public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

    BinaryTreeIterator(BinaryTreeNodePos<T> pos) : pos(pos) {}

    bool operator==(const BinaryTreeIterator& other) const {
        return (pos == other.pos);
    }

    auto operator<=>(const BinaryTreeIterator& other) const {
        return (pos <=> other.pos);
    }

    BinaryTreeIterator& operator++() {
        pos = ia.succ(pos);
        return *this;
    }

    BinaryTreeIterator operator++(int) {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    BinaryTreeIterator& operator--() {
        pos = ia.pred(pos);
        return *this;
    }

    BinaryTreeIterator operator--(int) {
        auto temp = *this;
        --(*this);
        return temp;
    }

    BinaryTreeIterator& operator+=(int offset) {
        while (offset-- > 0) {
            ++(*this);
        }
        return *this;
    }

    BinaryTreeIterator& operator-=(int offset) {
        while (offset-- > 0) {
            --(*this);
        }
        return *this;
    }

    BinaryTreeIterator operator+(int offset) {
        auto temp = *this;
        temp += offset;
        return temp;
    }

    BinaryTreeIterator operator-(int offset) {
        auto temp = *this;
        temp -= offset;
        return temp;
    }

    reference operator*() const {
        return pos->data();
    }

    pointer operator->() const {
        return &(pos->data());
    }

    BinaryTreeNodePos<T> getPosition() const {
        return pos;
    }

    // 重载复制构造函数和移动构造函数
    BinaryTreeIterator(const BinaryTreeIterator& other) : pos(other.pos) {}
    BinaryTreeIterator(BinaryTreeIterator&& other): pos(other.pos) {}
    BinaryTreeIterator& operator=(const BinaryTreeIterator& other) {
        pos = other.pos;
        return *this;
    }
    BinaryTreeIterator& operator=(BinaryTreeIterator&& other) {
        pos = other.pos;
        return *this;
    }
};

}