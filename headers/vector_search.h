#pragma once

#include "vector.h"

namespace clazy_framework {

// 这个文件考虑一个基本问题：有序向量的查找
// 返回一个pair，bool表示是否查找成功
// Rank在成功时，表示查找到的元素的秩；在失败时，指示应当被插入的位置
template <typename T>
class VectorSearch : public Algorithm {
protected:
    virtual pair<bool, Rank> search(Vector<T>& V, const T& e, const function<bool(const T&, const T&)>& cmp) = 0;
public:
    virtual pair<bool, Rank> apply(Vector<T>& V, const T& e, const function<bool(const T&, const T&)>& cmp) {
        return search(V, e, cmp);
    }
    virtual pair<bool, Rank> apply(Vector<T>& V, const T& e) {
        return search(V, less_equal<T>());
    }
};

}

namespace clazy {

// 折半查找（迭代方法）
template <typename T>
class VectorBinarySearch : public VectorSearch<T> {
protected:
    virtual Vector<T>::Iterator binarySearch(Vector<T>::Iterator it_begin, Vector<T>::Iterator it_end, const T& e, const function<bool(const T&, const T&)>& cmp);
    virtual pair<bool, Rank> search(Vector<T>& V, const T& e, const function<bool(const T&, const T&)>& cmp) {
        auto pos = binarySearch(begin(V), end(V), e, cmp);
        if (pos > begin(V) && *(pos - 1) == e) {
            return {true, pos - begin(V) - 1};
        } else {
            return {false, pos};
        }
    }
};

// 迭代方法（对应邓俊辉《数据结构》形式C）
template <typename T>
Vector<T>::Iterator VectorBinarySearch<T>::binarySearch(Vector<T>::Iterator it_begin, Vector<T>::Iterator it_end, const T& e, const function<bool(const T&, const T&)>& cmp) {
    while (it_begin < it_end) {        // 保证it_begin左侧的元素不小于e，it_end及右侧的元素大于e
        auto it_mid = it_begin + (it_end - it_begin) / 2; // 取中点
        if (cmp(*it_mid, e)) {         // 如果e不小于中点 
            it_begin = it_mid + 1;     // 则进入右半部分查找，注意需要+1避免无限循环
        } else {                       // 如果e小于中点
            it_end = it_mid;           // 则进入左半部分查找
        }
    }
    return it_begin;                   // 如果查找成功，则it_begin-1是秩最大的等于e的元素；如果查找失败，it_begin是e的插入位置
}

}