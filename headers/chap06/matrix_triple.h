#pragma once

#include "matrix_framework.h"
#include "triple_framework.h"
#include "vector.h"
#include "search_binary.h" 

namespace clazy {

// 在矩阵中使用的三元组普通的三元组有所区别
template <typename T>
class TripleElement : public clazy_framework::Triple<T> {
public:
    TripleElement() {}
    TripleElement(int r, int c): clazy_framework::Triple<T>(r, c) {}
    TripleElement(int r, int c, const T& v): clazy_framework::Triple<T>(r, c, v) {}

    bool operator==(const TripleElement<T>& other) const {
        return this->r == other.r && this->c == other.c;
    }

    bool operator<=(const TripleElement<T>& other) const {
        if (this->r < other.r) {
            return true;
        } else if (this->r == other.r) {
            return this->c <= other.c;
        } else {
            return false;
        }
    }
};

// 采用有序三元组的方法存储矩阵
// 用于存储非常稀疏（以及不容易变化的矩阵）
template <int R, int C = R, typename T = int>
class MatrixTriple : public clazy_framework::AbstractMatrix<R, C, T> {
private:
	using Triple = TripleElement<T>;
    Vector<Triple> _data;

public:
    T item(int r, int c) const override {
        auto [result, rank] = BinarySearch<Triple>()(_data, Triple(r, c));
        if (result) {
            return _data[rank].v;
        } else {
            return 0;
        }
    }

    void set(int r, int c, const T& v) override {
        auto [result, rank] = BinarySearch<Triple>()(_data, Triple(r, c));
        if (result) {
            _data[rank].v = v;
        } else {
            _data.insert(rank, Triple(r, c, v));
        }
    }

    void unset(int r, int c) override {
        auto [result, rank] = BinarySearch<Triple>()(_data, Triple(r, c));
        if (result) {
            _data.remove(rank);
        }
    }

    int size() const override {
        return _data.size();
    }

    void clear() override {
        _data.clear();
    }
};

}
