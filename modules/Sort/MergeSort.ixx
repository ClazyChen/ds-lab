module;
#include <algorithm>
#include <type_traits>

export module Sort.MergeSort;

export import :ListMergeSort;

import LinearList;
import Vector;
import Sort.AbstractSort;

export namespace dslab {

template <typename T, template<typename> typename Linear>
class MergeSort : public AbstractSort<T, Linear> {
protected:
    Vector<T> W;
    using Iterator = typename Linear<T>::iterator;
    void merge(Iterator lo, Iterator mi, Iterator hi, size_t leftSize) {
        W.resize(leftSize);
        std::move(lo, mi, std::begin(W));
        auto i { std::begin(W)};
        auto j { mi }, k { lo };
        while (i != std::end(W) && j != hi) {
            if (this->cmp(*j, *i)) {
                *k++ = std::move(*j++);
            } else {
                *k++ = std::move(*i++);
            }
        }
        std::move(i, std::end(W), k);
    }
    void mergeSort(Iterator lo, Iterator hi, size_t size) {
        if (size < 2) return;
        auto mi { lo + size / 2 };
        mergeSort(lo, mi, size / 2);
        mergeSort(mi, hi, size - size / 2);
        merge(lo, mi, hi, size / 2);
    }
public:
    void sort(Linear<T>& L) override {
        mergeSort(std::begin(L), std::end(L), L.size());
    }
};

template <typename T, template<typename> typename Linear>
class MergeSortUpward : public MergeSort<T, Linear> {
public:
    void operator()(Linear<T>& L) override {
        auto n { L.size() };
        for (size_t width { 1 }; width < n; width *= 2) {
            auto lo { std::begin(L) };
            Rank i { 2 * width };
            while (i < n) {
                auto mi { lo + width };
                auto hi { mi + width };
                this->merge(lo, mi, hi, width);
                lo = hi;
                i += 2 * width;
            }
            if (n + width > i) {
                auto mi { lo + width };
                auto hi { std::end(L) };
                this->merge(lo, mi, hi, width);
            }
        }
    }
};

}
