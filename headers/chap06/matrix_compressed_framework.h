#pragma once

#include "matrix_framework.h"

// 矩阵的压缩存储
// 这个部分仅用于408的一些考点上

// 这个文件展示了用一维向量存储压缩矩阵的一个模板
// 在它的基础上，只需要改写item函数和set函数就可以生成各种形式的矩阵

namespace clazy_framework {

// 基本的压缩矩阵，D表示实际存放的元素数量
template <int R, int C, int D, typename T>
class AbstractCompressedMatrix : public clazy_framework::AbstractMatrix<R, C, T> {
protected:
    T _data[D];
    void copyMatrix(const AbstractCompressedMatrix<R, C, D, T>& other) {
        if (this != &other) {
            for (int i = 0; i < D; i++) {
                this->_data[i] = other._data[i];
            }
        }
    }

public:
    virtual T item(int r, int c) const = 0;
    virtual void set(int r, int c, const T& v) = 0;

    int size() const override {
        int count = 0;
        for (int i = 0; i < D; i++) {
            if (this->_data[i] != 0) {
                count++;
            }
        }
        return count;
    }

    void clear() override {
        for (int i = 0; i < D; i++) {
            this->_data[i] = 0;
        }
    }

    AbstractCompressedMatrix() {}
    AbstractCompressedMatrix(const AbstractCompressedMatrix<R, C, D, T>& other) {
        copyMatrix(other);
    }
    AbstractCompressedMatrix(AbstractCompressedMatrix&& other) {
        copyMatrix(other);
    }
    AbstractCompressedMatrix<R, C, D, T>& operator=(const AbstractCompressedMatrix<R, C, D, T>& other) {
        copyMatrix(other);
        return *this;
    }
    AbstractCompressedMatrix<R, C, D, T>& operator=(AbstractCompressedMatrix<R, C, D, T>&& other) {
        copyMatrix(other);
        return *this;
    }
};

}