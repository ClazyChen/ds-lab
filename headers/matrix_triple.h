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
    Triple() {}
    Triple(int r, int c): r(r), c(c) {}
    Triple(int r, int c, const T& v): r(r), c(c), v(v) {}
    Triple(const Triple<T>& other): r(other.r), c(other.c), v(other.v) {}
    Triple(Triple<T>&& other): r(other.r), c(other.c), v(move(other.v)) {}
    Triple<T>& operator=(const Triple<T>& other) {
        r = other.r;
        c = other.c;
        v = other.v;
        return *this;
    }
    Triple<T>& operator=(Triple<T>&& other) {
        r = other.r;
        c = other.c;
        v = move(other.v);
        return *this;
    }

    bool operator==(const Triple<T>& other) const {
        return r == other.r && c == other.c;
    }
    auto operator<=>(const Triple<T>& other) const = default;
};

// 采用有序三元组的方法存储矩阵
// 用于存储非常稀疏（以及不容易变化的矩阵）
template <int R, int C = R, typename T = int>
class MatrixTriple : public clazy_framework::AbstractMatrix<R, C, T> {
protected:
    Vector<Triple<T>> data;
    VectorBinarySearch<Triple<T>> search;
    void copyMatrix(const MatrixTriple<R, C, T>& other) {
        if (this != &other) {
            data = other.data;
            search = other.search;
        }
    }
    void moveMatrix(MatrixTriple<R, C, T>&& other) {
        if (this != &other) {
            data = move(other.data);
            search = move(other.search);
        }
    }

public:
    virtual T item(int r, int c) const override {
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

    MatrixTriple() {}
    MatrixTriple(const MatrixTriple<R, C, T>& other) {
        copyMatrix(other);
    }
    MatrixTriple(MatrixTriple<R, C, T>&& other) {
        moveMatrix(other);
    }
    MatrixTriple<R, C, T>& operator=(const MatrixTriple<R, C, T>& other) {
        copyMatrix(other);
        return *this;
    }
    MatrixTriple<R, C, T>& operator=(MatrixTriple<R, C, T>&& other) {
        moveMatrix(other);
        return *this;
    }
};


}
