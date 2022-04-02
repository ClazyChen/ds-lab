#pragma once

#include "matrix_framework.h"

namespace clazy {

// 矩阵的压缩存储
// 这个部分仅用于408的一些考点上

// 特别注意，在这个文件中的所有示例代码
// 都服从AbstractMatrix的接口定义
// 所以，这个矩阵的行和列都是从0开始的
// 可以发现，当都使用从0开始计算的方法的时候，公式比较简单
// 但考试的时候，不一定是这样

// 这个文件里核心的部分是item和set
// 其他的构造函数相关的内容，可以忽略不计
// 和matrix.h里的示例代码思路是一致的

// 基本的压缩矩阵，D表示实际存放的元素数量
template <int R, int C, int D, typename T>
class AbstractCompressedMatrix : public clazy_framework::AbstractMatrix<R, C, T> {
protected:
    T data[D];
    void copyMatrix(const AbstractCompressedMatrix<R, C, D, T>& other) {
        if (this != &other) {
            for (int i : views::iota(0, D)) {
                this->data[i] = other.data[i];
            }
        }
    }
    void moveMatrix(AbstractCompressedMatrix<R, C, D, T>&& other) {
        if (this != &other) {
            for (int i : views::iota(0, D)) {
                this->data[i] = move(other.data[i]);
            }
        }
    }
};

// 行优先的矩阵
template <int R, int C = R, typename T = int>
class RowMajorMatrix : public AbstractCompressedMatrix<R, C, R * C, T> {
public:
    virtual T item(int r, int c) const override {
        return data[r * C + c];
    }

    virtual void set(int r, int c, const T& v) override {
        data[r * C + c] = v;
    }

    RowMajorMatrix() {}
    RowMajorMatrix(const RowMajorMatrix<R, C, T>& other) {
        copyMatrix(other);
    }
    RowMajorMatrix(RowMajorMatrix<R, C, T>&& other) {
        moveMatrix(other);
    }
    auto& operator=(const RowMajorMatrix<R, C, T>& other) {
        copyMatrix(other);
        return *this;
    }
    auto& operator=(RowMajorMatrix<R, C, T>&& other) {
        moveMatrix(other);
        return *this;
    }
};

// 列优先的矩阵
template <int R, int C = R, typename T = int>
class ColMajorMatrix : public AbstractCompressedMatrix<R, C, R * C, T> {
public:
    virtual T item(int r, int c) const override {
        return data[c * R + r];
    }

    virtual void set(int r, int c, const T& v) override {
        data[c * R + r] = v;
    }

    ColMajorMatrix() {}
    ColMajorMatrix(const ColMajorMatrix<R, C, T>& other) {
        copyMatrix(other);
    }
    ColMajorMatrix(ColMajorMatrix<R, C, T>&& other) {
        moveMatrix(other);
    }
    auto& operator=(const ColMajorMatrix<R, C, T>& other) {
        copyMatrix(other);
        return *this;
    }
    auto& operator=(ColMajorMatrix<R, C, T>&& other) {
        moveMatrix(other);
        return *this;
    }
};

// 对称矩阵，只存储半个矩阵
template <int N, typename T = int>
class SymmetricMatrix : public AbstractCompressedMatrix<N, N, N * (N + 1) / 2, T> {
public:
    virtual T item(int r, int c) const override {
        if (r > c) {
            swap(r, c);
        }
        return data[(r * (r + 1)) / 2 + c];
    }

    virtual void set(int r, int c, const T& v) override {
        if (r > c) {
            swap(r, c);
        }
        data[(r * (r + 1)) / 2 + c] = v;
    }

    SymmetricMatrix() {}
    SymmetricMatrix(const SymmetricMatrix<N, T>& other) {
        copyMatrix(other);
    }
    SymmetricMatrix(SymmetricMatrix<N, T>&& other) {
        moveMatrix(other);
    }
    auto& operator=(const SymmetricMatrix<N, T>& other) {
        copyMatrix(other);
        return *this;
    }
    auto& operator=(SymmetricMatrix<N, T>&& other) {
        moveMatrix(other);
        return *this;
    }
};

// 下三角矩阵
// 这里是一般的数学的下三角矩阵，也就是上三角区全是0的情况
template <int N, typename T = int>
class LowerTriangularMatrix : public AbstractCompressedMatrix<N, N, N * (N + 1) / 2, T> {
public:
    virtual T item(int r, int c) const override {
        if (r > c) { // 上三角区总是返回0
            return (T)0;
        }
        return data[(r * (r + 1)) / 2 + c];
    }

    virtual void set(int r, int c, const T& v) override { // 不允许对上三角区进行赋值
        if (r <= c) {
            data[(r * (r + 1)) / 2 + c] = v;
        }
    }

    LowerTriangularMatrix() {}
    LowerTriangularMatrix(const LowerTriangularMatrix<N, T>& other) {
        copyMatrix(other);
    }
    LowerTriangularMatrix(LowerTriangularMatrix<N, T>&& other) {
        moveMatrix(other);
    }
    auto& operator=(const LowerTriangularMatrix<N, T>& other) {
        copyMatrix(other);
        return *this;
    }
    auto& operator=(LowerTriangularMatrix<N, T>&& other) {
        moveMatrix(other);
        return *this;
    }
};

// 上三角矩阵
// 这里是一般的数学的上三角矩阵，也就是下三角区全是0的情况
template <int N, typename T = int>
class UpperTriangularMatrix : public AbstractCompressedMatrix<N, N, N * (N + 1) / 2, T> {
public:
    virtual T item(int r, int c) const override {
        if (r < c) { // 下三角区总是返回0
            return (T)0;
        }
        return data[(r * (2 * N - 1 - r)) / 2 + c];
    }

    virtual void set(int r, int c, const T& v) override { // 不允许对下三角区进行赋值
        if (r >= c) {
            data[(r * (2 * N - 1 - r)) / 2 + c] = v;
        }
    }

    UpperTriangularMatrix() {}
    UpperTriangularMatrix(const UpperTriangularMatrix<N, T>& other) {
        copyMatrix(other);
    }
    UpperTriangularMatrix(UpperTriangularMatrix<N, T>&& other) {
        moveMatrix(other);
    }
    auto& operator=(const UpperTriangularMatrix<N, T>& other) {
        copyMatrix(other);
        return *this;
    }
    auto& operator=(UpperTriangularMatrix<N, T>&& other) {
        moveMatrix(other);
        return *this;
    }
};

// 三对角矩阵
template <int N, typename T = int>
class TridiagonalMatrix : public AbstractCompressedMatrix<N, N, 3 * N - 2, T> {
public:
    virtual T item(int r, int c) const override {
        if (abs(r - c) > 1) {
            return (T)0;
        }
        return data[2 * r + c];
    }
    
    virtual void set(int r, int c, const T& v) override {
        if (abs(r - c) > 1) {
            return;
        }
        data[2 * r + c] = v;
    }

    TridiagonalMatrix() {}
    TridiagonalMatrix(const TridiagonalMatrix<N, T>& other) {
        copyMatrix(other);
    }
    TridiagonalMatrix(TridiagonalMatrix<N, T>&& other) {
        moveMatrix(other);
    }
    auto& operator=(const TridiagonalMatrix<N, T>& other) {
        copyMatrix(other);
        return *this;
    }
    auto& operator=(TridiagonalMatrix<N, T>&& other) {
        moveMatrix(other);
        return *this;
    }
};

}