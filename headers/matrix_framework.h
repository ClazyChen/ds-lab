#pragma once

#include "framework.h"

namespace clazy_framework {

// 矩阵在清华的体系里面，好像是放在离散数学和数值分析里的
// 并没有放在数据结构里，不过在408的体系里面有，所以还是放在这里作为单独的一章

// 这个文件定义了一个抽象的矩阵类
// 矩阵类的抽象是指定了矩阵的行数和列数，这是不可变的
// 两个参数分别是行数和列数
template <int R, int C, typename T>
requires (R > 0 && C > 0 && is_convertible_v<int, T>)
class AbstractMatrix : public DataStructure<T> {
public:
    constexpr int getRows() const { return R; }
    constexpr int getCols() const { return C; }
    virtual T item(int r, int c) const = 0; // 获取矩阵的某个元素
    virtual void set(int r, int c, const T& v) = 0; // 设置矩阵的某个元素
    virtual void unset(int r, int c) { set(r, c, 0); } // 取消矩阵的某个元素，设置为0

    // 矩阵没有清空的操作，也就始终认为是非空的
    virtual void clear() override {}
    virtual bool empty() const override { return false; }
};

// 矩阵的统一输出接口
template <int R, int C, typename T>
ostream& operator<<(ostream& out, const AbstractMatrix<R, C, T>& matrix) {
    for (int r : views::iota(0, R)) {
        out << "[";
        for (bool first = true; int c : views::iota(0, C)) {
            if (!first) {
                out << ", ";
            }
            out << matrix.item(r, c);
            first = false;
        }
        out << "]" << endl;
    }
    return out;
}

}