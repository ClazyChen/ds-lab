#pragma once

#include "matrix_framework.h"

// 一个基本的矩阵，不带任何压缩

namespace clazy {

template <int R, int C = R, typename T = int>
class Matrix : public clazy_framework::AbstractMatrix<R, C, T> {
protected:
    T data[R][C];
public:
    virtual T item(int r, int c) const override {
        return data[r][c];
    }

    virtual void set(int r, int c, const T& v) override {
        data[r][c] = v;
    }
};

}