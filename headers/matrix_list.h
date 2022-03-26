#pragma once

#include "matrix_framework.h"
#include "vector.h"
#include "list.h"

namespace clazy {

// 利用向量套列表的形式，实现一个基本的矩阵
// 这种方法可以被用来实现邻接表
template <int R, int C = R, typename T = int>
requires (R > 0 && C > 0 && is_convertible_v<int, T>)
class MatrixList : public clazy_framework::AbstractMatrix<R, C, T> {
protected:
    ForwardList<pair<int, T>> data[R];
public:
    virtual T item(int r, int c) override { // 在列表中查找c
        for (auto it = begin(data[r]); it != end(data[r]); it++) {
            if (it->first == c) {
                return it->second;
            }
        }
        return (T)0;
    }

    virtual void set(int r, int c, const T& v) override {
        for (auto it = begin(data[r]); it != end(data[r]); it++) {
            if (it->first == c) {
                it->second = v;
                return;
            }
        }
        data[r].push_back({c, v});
    }
};

}