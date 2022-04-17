#pragma once

// 这个文件讨论查找这个问题
// 这里讨论两种类型的查找
// 1. 有序的查找，包括：
//    a. 线性查找
//    b. 折半查找（向量上）
//    c. 分块查找（分块表上）
//    d. 树形查找（BST、B树、B+树、跳表）
// 2. 无序的查找，包括：
//    a. 哈希查找（哈希表）
// 注意，在这里是没有做伸展树的特化的
// 因为伸展树会改动数据结构，在这里的查找，伸展树和AVL树、红黑树一样作为普通的BST处理

// 目前的设计在这一部分不使用迭代器模式

#include "framework.h"

namespace clazy_framework {

// 查找操作返回一个pair，其中bool表示是否查找到，P表示查找结果
template <typename P>
using SearchResult = pair<bool, P>;

// 对于下面的模板，用户只需要重载search函数即可

// 基本的查找
template <typename T, typename P, typename Container>
requires (is_data_structure<T, Container>)
class AbstractSearch : public Algorithm<SearchResult<P>, const Container&, const T&> {
protected:
    virtual SearchResult<P> search(const Container& C, const T& e) = 0;
public:
    SearchResult<P> apply(const Container& C, const T& e) override {
        return search(C, e);
    }
};

// 有序的查找，基本模板
template <typename T, typename P, typename Container>
class AbstractOrderedSearch : public AbstractSearch<T, P, Container> {
protected:
    virtual SearchResult<P> search(const Container& C, const T& e, const Comparator<T>& cmp) = 0;
    SearchResult<P> search(const Container& C, const T& e) override {
        return search(C, e, less_equal<T>());
    }
public:
    SearchResult<P> apply(const Container& C, const T& e, const Comparator<T>& cmp = less_equal<T>()) {
        return search(C, e, cmp);
    }
    SearchResult<P> operator()(const Container& C, const T& e, const Comparator<T>& cmp = less_equal<T>()) {
        return search(C, e, cmp);
    }
};

}
