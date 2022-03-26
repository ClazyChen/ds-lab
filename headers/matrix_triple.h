#pragma once

#include "matrix_framework.h"
#include "vector_search.h"

namespace clazy {

// 三元组
template <typename T>
class Triple {
public:
    int r, c;
    T v;
    Triple(int r, int c): r(r), c(c) {}
    Triple(int r, int c, const T& v): r(r), c(c), v(v) {}
    bool operator==(const Triple& other) const {
        return r == other.r && c == other.c;
    }
    bool operator<=>(const Triple& other) const = default;
};

// 采用有序三元组的方法存储矩阵
// 用于存储非常稀疏（以及不容易变化的矩阵）
template <int R, int C = R, typename T = int>
class MatrixTriple : public clazy_framework::AbstractMatrix<R, C, T> {
protected:
    Vector<Triple<T>> data;
    VectorBinarySearch<Triple<T>> search;
public:
    virtual T item(int r, int c) override {
        auto [result, rank] = search.apply(data, Triple<T>(r, c));
        if (result) {
            return data[rank].v;
        } else {
            return (T)0;
        }
    }

    virtual void set(int r, int c, const T& v) override {
        auto [result, rank] = search.apply(data, Triple<T>(r, c));
        if (result) {
            data[rank].v = v;
        } else {
            data.insert(rank, Triple<T>(r, c, v));
        }
    }

};


}