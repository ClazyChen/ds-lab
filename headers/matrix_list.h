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
    virtual T item(int r, int c) const override { // 在列表中查找c
        for (auto it = begin(data[r]); it != end(data[r]); it++) {
            if (it->data().first == c) {
                return it->data().second;
            }
        }
        return (T)0;
    }

    virtual void set(int r, int c, const T& v) override {
        for (auto it = begin(data[r]); it != end(data[r]); it++) {
            if (it->data().first == c) {
                it->data().second = v;
                return;
            }
        }
        data[r].push_back({c, v});
    }

    virtual void unset(int r, int c) override { // 重载unset，不需要的元素就直接删除
        for (auto it = begin(data[r]); it != end(data[r]); it++) {
            if (it->data().first == c) {
                data[r].remove(it.base());
                return;
            }
        }
    }
};

}