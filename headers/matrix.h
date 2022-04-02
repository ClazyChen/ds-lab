#pragma once

#include "matrix_framework.h"

// 一个基本的矩阵，不带任何压缩

namespace clazy {

template <int R, int C = R, typename T = int>
class Matrix : public clazy_framework::AbstractMatrix<R, C, T> {
protected:
    T data[R][C];
    void copyMatrix(const Matrix<R, C, T>& other) {
        if (this != &other) {
            for (int r : views::iota(0, R)) {
                for (int c : views::iota(0, C)) {
                    this->data[r][c] = other.data[r][c];
                }
            }
        }
    }
    void moveMatrix(Matrix<R, C, T>&& other) {
        if (this != &other) {
            for (int r : views::iota(0, R)) {
                for (int c : views::iota(0, C)) {
                    this->data[r][c] = std::move(other.data[r][c]);
                }
            }
        }
    }

public:
    virtual T item(int r, int c) const override {
        return data[r][c];
    }

    virtual void set(int r, int c, const T& v) override {
        data[r][c] = v;
    }

    Matrix() {}
    Matrix(const Matrix<R, C, T>& other) {
        copyMatrix(other);
    }
    Matrix(const Matrix<R, C, T>&& other) {
        moveMatrix(other);
    }
    auto& operator=(const Matrix<R, C, T>& other) {
        copyMatrix(other);
        return *this;
    }
    auto& operator=(Matrix<R, C, T>&& other) {
        moveMatrix(other);
        return *this;
    }
};

}