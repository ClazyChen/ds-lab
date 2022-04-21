#pragma once

#include "matrix_framework.h"
#include "list_forward.h"

// 利用列表数组的形式，实现一个基本的矩阵
// 这种方法可以被用来实现邻接表

namespace clazy {

template <int R, int C = R, typename T = int>
class MatrixList : public clazy_framework::AbstractMatrix<R, C, T> {
protected:
    ForwardList<pair<int, T>> _data[R];
    void copyMatrix(const MatrixList<R, C, T>& other) {
        if (this != &other) {
            for (int r = 0; r < R; r++) {
                _data[r] = other._data[r];
            }
        }
    }
    void moveMatrix(MatrixList<R, C, T>&& other) {
        if (this != &other) {
            for (int r = 0; r < R; r++) {
                _data[r] = move(other._data[r]);
            }
        }
    }

public:
    T item(int r, int c) const override { // 在列表中查找c
        if (auto it = find_if(_data[r].begin(), _data[r].end(), [c](auto& p) { return p.first == c; }); it != _data[r].end()) {
            return it->second;
        } else {
            return 0;
        }
    }

    void set(int r, int c, const T& v) override {
        if (auto it = find_if(_data[r].begin(), _data[r].end(), [c](auto& p) { return p.first == c; }); it != _data[r].end()) {
            it->second = v;
        } else {
            _data[r].push_back({c, v});
        }
    }

    void unset(int r, int c) override { // 重载unset，不需要的元素就直接删除
        remove_if(_data[r].begin(), _data[r].end(), [c](auto& p) { return p.first == c; });
    }

    int size() const override {
        int count = 0;
        for (int r = 0; r < R; r++) {
            count += _data[r].size();
        }
        return count;
    }

    void clear() override {
        for (int r = 0; r < R; r++) {
            _data[r].clear();
        }
    }

    MatrixList() {}
    MatrixList(const MatrixList<R, C, T>& other) {
        copyMatrix(other);
    }
    MatrixList(MatrixList<R, C, T>&& other) {
        moveMatrix(other);
    }
    auto& operator=(const MatrixList<R, C, T>& other) {
        copyMatrix(other);
        return *this;
    }
    auto& operator=(MatrixList<R, C, T>&& other) {
        moveMatrix(other);
        return *this;
    }

};

}
