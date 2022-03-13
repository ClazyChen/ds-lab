#pragma once

#include "vector.h"

namespace clazy_framework {

// 基本的查找
template <typename T, typename Container, typename P>
requires (is_base_of_v<DataStructure<T>, Container>)
class Search : public Algorithm {
protected:
    virtual pair<bool, P> search(Container& C, const T& e) = 0;
public:
    virtual pair<bool, P> apply(Container& C, const T& e) {
        return search(C, e);
    }
};

// 全序集上的查找
template <typename T, typename Container, typename P>
requires (is_base_of_v<DataStructure<T>, Container>)
class OrderedSearch : public Search<T, Container, P> {
protected:
    virtual pair<bool, P> search(Container& C, const T& e, const Comparator<T>& cmp) = 0;
    virtual pair<bool, P> search(Container& C, const T& e) { 
        return search(C, e, less_equal<T>());
    }
public:
    virtual pair<bool, P> apply(Container& C, const T& e, const Comparator<T>& cmp) {
        return search(C, e, cmp);
    }
};

// 这个文件考虑一个基本问题：有序向量的查找
// 返回一个pair，bool表示是否查找成功
// Rank在成功时，表示查找到的元素的秩；在失败时，指示应当被插入的位置
template <typename T, typename Vector = clazy::Vector<T>>
requires (is_base_of_v<AbstractVector<T>, Vector>)
class VectorSearch : public Algorithm {
protected:
    virtual pair<bool, Rank> search(Vector& V, const T& e, const function<bool(const T&, const T&)>& cmp) = 0;
public:
    virtual pair<bool, Rank> apply(Vector& V, const T& e, const function<bool(const T&, const T&)>& cmp = less_equal<T>()) {
        return search(V, e, cmp);
    }
};

}

namespace clazy {

// 顺序查找
template <typename T>
class VectorSequentialSearch : public clazy_framework::VectorSearch<T> {
protected:
    virtual pair<bool, Rank> search(Vector<T>& V, const T& e, const function<bool(const T&, const T&)>& cmp) {
        for (Rank r = V.size()-1; r >= 0; r--) {
            if (cmp(V[r], e)) {
                if (V[r] == e) {
                    return {true, r};
                } else {
                    return {false, r+1};
                }
            }
        }
        return {false, 0};
    }
};

// 折半查找（迭代方法）
template <typename T>
class VectorBinarySearch : public clazy_framework::VectorSearch<T> {
protected:
    virtual VectorIterator<T> binarySearch(VectorIterator<T> it_begin, VectorIterator<T> it_end, const T& e, const function<bool(const T&, const T&)>& cmp);
    virtual pair<bool, Rank> search(Vector<T>& V, const T& e, const function<bool(const T&, const T&)>& cmp) {
        auto pos = binarySearch(begin(V), end(V), e, cmp);
        if (pos > begin(V) && *(pos - 1) == e) {
            return {true, pos - begin(V) - 1};
        } else {
            return {false, pos - begin(V)};
        }
    }
};

// 迭代方法（对应邓俊辉《数据结构》形式C）
template <typename T>
VectorIterator<T> VectorBinarySearch<T>::binarySearch(VectorIterator<T> it_begin, VectorIterator<T> it_end, const T& e, const function<bool(const T&, const T&)>& cmp) {
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
