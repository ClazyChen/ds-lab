#pragma once

// 这个文件考虑一个基本问题：排序
// 被排序的元素必须是可以比较的，至少定义了<=这个操作符

#include "framework.h"

namespace clazy_framework {

template <typename T, typename Container>
requires (is_data_structure<T, Container>)
class AbstractSort : public Algorithm<void, Container&, const Comparator<T>&> {
protected:
    virtual void sort(Container& C, const Comparator<T>& cmp) = 0;
public:
    void apply(Container& C, const Comparator<T>& cmp = less_equal<T>()) override {
        sort(C, cmp);
    }
    void operator()(Container& C, const Comparator<T>& cmp = less_equal<T>()) {
        sort(C, cmp);
    }
};

}

