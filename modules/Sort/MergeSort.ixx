module;
#include <algorithm>
#include <type_traits>

export module Sort.MergeSort;

export import :ListMergeSort;

import LinearList;
import Vector;
import Sort.AbstractSort;

export namespace dslab {

template <typename T, template<typename> typename Linear, typename Comparator = std::less<T>>
class MergeSort : public AbstractSort<T, Linear, Comparator> {
protected:
    Vector<T> W;
    Comparator cmp;
    using Iterator = typename Linear<T>::iterator;
    void merge(Iterator lo, Iterator mi, Iterator hi, size_t size) {
        W.resize(size / 2);
        std::move(lo, mi, W.begin());
        auto i { W.begin() };
        auto j { mi }, k { lo };
        while (i != W.end() && j != hi) {
            if (cmp(*j, *i)) {
                *k++ = std::move(*j++);
            } else {
                *k++ = std::move(*i++);
            }
        }
        std::move(i, W.end(), k);
    }
    void mergeSort(Iterator lo, Iterator hi, size_t size) {
        if (size < 2) return;
        auto mi { lo + size / 2 };
        mergeSort(lo, mi, size / 2);
        mergeSort(mi, hi, size - size / 2);
        merge(lo, mi, hi, size);
    }
public:
    void operator()(Linear<T>& L) override {
        mergeSort(std::begin(L), std::end(L), L.size());
    }
};

}
