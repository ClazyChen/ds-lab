#pragma once

#include "framework.h"

namespace clazy_framework {

// 这个文件讨论查找这个问题
// 查找操作返回一个pair，其中bool表示是否查找到，P表示查找结果
template <typename P>
using SearchResult = pair<bool, P>;

// 基本的查找
template <typename T, typename P, typename Container>
requires (is_data_structure<T, Container>)
class Search : public Algorithm {
protected:
    virtual SearchResult<P> search(const Container& C, const T& e) const = 0;
public:
    SearchResult<P> apply(const Container& C, const T& e) const {
        return search(C, e);
    }
};

// 全序集上的查找（基于比较函数）
// 在向量、列表、分块表、BST、B树、B+树、跳表上做查找都使用这个
// 散列是基于散列函数的，不使用这个
template <typename T, typename P, typename Container>
requires (is_data_structure<T, Container>)
class OrderedSearch : public Search<T, P, Container> {
protected:
    virtual SearchResult<P> search(const Container& C, const T& e, const Comparator<T>& cmp) const = 0;
    virtual SearchResult<P> search(const Container& C, const T& e) const override { // 默认
        return search(C, e, less_equal<T>());
    }
public:
    SearchResult<P> apply(const Container& C, const T& e, const Comparator<T>& cmp = less_equal<T>()) const {
        return search(C, e, cmp);
    }
};

}
