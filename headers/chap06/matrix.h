#pragma once

#include "matrix_framework.h"

// 一个基本的矩阵，不带任何压缩
// 这里没有设置单独的变量来维护size，为了让set的性能尽可能高
// 而size在矩阵操作中并不是一个经常的事情

namespace clazy {

template <int R, int C = R, typename T = int>
class Matrix : public clazy_framework::AbstractMatrix<R, C, T> {
private:
    T _data[R][C];
    void copyMatrix(const Matrix<R, C, T>& other) {
        if (this != &other) {
            for (int r = 0; r < R; r++) {
                for (int c = 0; c < C; c++) {
                    _data[r][c] = other._data[r][c];
                }
            }
        }
    }
public:
    T item(int r, int c) const override {
        return _data[r][c];
    }

    void set(int r, int c, const T& v) override {
        _data[r][c] = v;
    }

    int size() const override {
        int count = 0;
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (_data[r][c] != 0) {
                    count++;
                }
            }
        }
        return count;
    }

    void clear() override {
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                _data[r][c] = 0;
            }
        }
    }

    Matrix() {
        clear();
    }
    Matrix(const Matrix<R, C, T>& other) {
        copyMatrix(other);
    }
    Matrix(const Matrix<R, C, T>&& other) {
        copyMatrix(other);
    }
    auto& operator=(const Matrix<R, C, T>& other) {
        copyMatrix(other);
        return *this;
    }
    auto& operator=(Matrix<R, C, T>&& other) {
        copyMatrix(other);
        return *this;
    }
};

}