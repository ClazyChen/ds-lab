#pragma once

#include "framework.h"

// 矩阵在清华的体系里面，是放在离散数学和数值分析里的
// 并没有放在数据结构里，不过在408的体系里面有，所以还是放在这里作为单独的一章

// 这个文件定义了一个抽象的矩阵类
// 矩阵类的抽象是指定了矩阵的行数和列数，这是不可变的
// 两个参数分别是行数和列数
// 矩阵需要支持的操作
// 1. 获取矩阵的某个元素
// 2. 设置矩阵的某个元素
// 3. 取消矩阵的某个元素（设置为0）
// 4. 获取矩阵的规模（非0元素的数量）
// 5. 清空矩阵（全部设置为0）

namespace clazy_framework {

template <int R, int C, typename T>
requires (R > 0 && C > 0 && is_convertible_v<int, T>)
class AbstractMatrix : public DataStructure<T> {
public:
    constexpr int countRows() const { return R; }
    constexpr int countCols() const { return C; }
    
    virtual T item(int r, int c) const = 0; // 获取矩阵的某个元素
    virtual void set(int r, int c, const T& v) = 0; // 设置矩阵的某个元素
    virtual void unset(int r, int c) { set(r, c, 0); } // 取消矩阵的某个元素，设置为0
    virtual int size() const = 0;
    virtual void clear() = 0;
};

// 矩阵的统一输出接口
template <int R, int C, typename T>
ostream& operator<<(ostream& out, const AbstractMatrix<R, C, T>& matrix) {
    for (int r = 0; r < R; r++) {
        out << "[";
        for (int c = 0; c < C; c++) {
            out << matrix.item(r, c) << ", ";
        }
        out << "]" << endl;
    }
    return out;
}

}