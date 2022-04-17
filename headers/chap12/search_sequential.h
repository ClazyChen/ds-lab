#pragma once

// 这个文件讨论的是线性表的线性查找

#include "linear_framework.h"
#include "search_framework.h"

namespace clazy {

using clazy_framework::SearchResult;
using clazy_framework::Comparator;

template <typename T, typename P, typename Container>
requires (clazy_framework::is_linear_structure<T, P, Container>)
class SequentialSearch : public clazy_framework::AbstractOrderedSearch<T, P, Container> {
protected:
    SearchResult<P> search(const Container& C, const T& e, const Comparator<T>& cmp) override {
        for (auto it = begin(C); it != end(C); it++) {
            if (!cmp(*it, e)) {
                return {false, it.getPosition()};
            }
            if (cmp(e, *it)) {
                return {true, it.getPosition()};
            }
        }
        return {false, C.invalid()};
    }
};

}