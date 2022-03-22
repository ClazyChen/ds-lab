#pragma once

#include "framework.h"

namespace clazy_framework {

// 这个文件考虑一个基本问题：排序
template <typename T, typename Container>
requires (is_linear_structure<T, Container>)
class Sort : public Algorithm {
protected:
    virtual void sort(Container& C, const Comparator<T>& cmp) = 0;
public:
    virtual void apply(Container& C, const Comparator<T>& cmp = less_equal<T>()) {
        sort(C, cmp);
    }
};

}

