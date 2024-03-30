#pragma once

#include "AbstractSort.hpp"

namespace dslab::sort {

template <typename T, template<typename> typename L>
class MergeSort : public AbstractSort<T, L> {
protected:
    using iterator = typename L<T>::iterator;
    using AbstractSort<T, L>::cmp;

    void merge(iterator lo, iterator mi, iterator hi, std::size_t ls) {
        static Vector<T> W {};
        W.resize(ls);
        std::move(lo, mi, W.begin());
        auto i { W.begin() }, j { mi }, k { lo };
        while (i != W.end() && j != hi) {
            if (cmp(*j, *i)) {
                *k++ = std::move(*j++);
            } else {
                *k++ = std::move(*i++);
            }
        }
        std::move(i, W.end(), k);
    }
    void mergeSort(iterator lo, iterator hi, std::size_t size) {
        if (size < 2) return;
        auto mi { lo + size / 2 };
        mergeSort(lo, mi, size / 2);
        mergeSort(mi, hi, size - size / 2);
        merge(lo, mi, hi, size / 2);
    }
    void sort(L<T>& V) override {
        mergeSort(V.begin(), V.end(), V.size());
    }

public:
    std::string type_name() const override {
        return "Merge Sort (Downward)";
    }

};

template <typename T, template<typename> typename L>
class MergeSortUpward : public MergeSort<T, L> {
protected:
    using MergeSort<T, L>::merge;
    void sort(L<T>& V) override {
        auto n { V.size() };
        for (auto w { 1uz }; w < n; w *= 2) {
            auto lo { V.begin() };
            auto i { 2 * w };
            while (i < n) {
                auto mi { lo + w };
                auto hi { mi + w };
                merge(lo, mi, hi, w);
                lo = hi;
                i += 2 * w;
            }
            if (n + w > i) {
                auto mi { lo + w };
                auto hi { V.end() };
                merge(lo, mi, hi, w);
            }
        }
    }
public:
    std::string type_name() const override {
        return "Merge Sort (Upward)";
    }
};

}