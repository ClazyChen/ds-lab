#pragma once

#include "framework.h"

// 这个类用来实现三元组，用来存储稀疏矩阵
// 在《图》这一章中，它可以在图中表示一条带权的边
// 从而在Kruskal算法中进行排序

namespace clazy_framework {

// 三元组
template <typename T = int>
requires (is_convertible_v<int, T>)
class Triple : public Object {
public:
    int r, c;
    T v;
    Triple() {}
    Triple(int r, int c): r(r), c(c) {}
    Triple(int r, int c, const T& v): r(r), c(c), v(v) {}

    bool operator==(const Triple<T>& other) const {
        return v == other.v;
    }
    auto operator<=>(const Triple<T>& other) const {
        return v <=> other.v;
    }
};

template <typename T>
ostream& operator<< (ostream& out, const Triple<T>& triple) {
    out << "(" << triple.r << ", " << triple.c << ", " << triple.v << ")";
    return out;
}


}