#pragma once

// 这个文件将三种归并排序的实现统一起来
// 给出一个统一的接口，以便于统一调用

#include "sort_merge_vector.h"
#include "sort_merge_list.h"
#include "sort_merge_list_forward.h"

namespace clazy {

using clazy_framework::Comparator;

template <typename T, typename Container>
class MergeSort : public clazy_framework::AbstractSort<T, Container> {
protected:
    void sort(Container& V, const Comparator<T>& cmp) override {
        if constexpr (is_base_of_v<clazy_framework::AbstractVector<T>, Container>) {
            VectorMergeSort<T, Container> sorter;
            sorter.sort(V, cmp);
        } else if constexpr (is_base_of_v<clazy_framework::AbstractBidirectionalList<T>, Container>) {
            ListMergeSort<T, Container> sorter;
            sorter.sort(V, cmp);
        } else if constexpr (is_base_of_v<clazy_framework::AbstractForwardList<T>, Container>) {
            ForwardListMergeSort<T, Container> sorter;
            sorter.sort(V, cmp);
        }
    }
};

}