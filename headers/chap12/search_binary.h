#pragma once

// 这个文件给出了一个折半查找的实现
// 它基于邓俊辉《数据结构》里给出的实现C

#include "search_binary_framework.h"
#include "vector.h"

namespace clazy {

using clazy_framework::Comparator;

template <typename T, typename Container = Vector<T>>
class BinarySearch : public clazy_framework::AbstractBinarySearch<T, Container> {
protected:
    Rank binarySearch(const Container& C, const T& e, const Comparator<T>& cmp, Rank lo, Rank hi) override {
        while (lo < hi) {
            Rank mi = (lo + hi) / 2;
            if (cmp(C[mi], e)) {
                lo = mi + 1;
            } else {
                hi = mi;
            }
        }
        return lo;
    }
};

}