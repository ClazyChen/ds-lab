#pragma once

#include "AbstractSort.hpp"
#include "../list.hpp"

namespace dslab::sort {

template <typename T, template<typename> typename L>
requires std::is_base_of_v<AbstractList<T>, L<T>>
class ListMergeSort : public AbstractSort<T, L> {
    using node_ptr = std::unique_ptr<typename L<T>::node_type>;
    using iterator = typename L<T>::iterator;
    using AbstractSort<T, L>::cmp;
    constexpr node_ptr&& ptr(iterator pos) {
        return std::move((--pos).node()->next());
    }
    void connect(iterator& prev, node_ptr& next) {
        next->prev() = prev.node();
        prev.node()->next() = std::move(next);
        ++prev;
        next = std::move(prev.node()->next());
    }
    iterator merge(iterator lo, iterator mi, iterator hi) {
        auto plo { ptr(lo) }, pmi { ptr(mi) }, tail { ptr(hi) };
        auto head { lo - 1 }, last { head };
        while (plo && pmi) {
            if (cmp(pmi->data(), plo->data())) {
                connect(last, pmi);
            } else {
                connect(last, plo);
            }
        }
        while (plo) {
            connect(last, plo);
        }
        while (pmi) {
            connect(last, pmi);
        }
        tail->prev() = last.node();
        last.node()->next() = std::move(tail);
        return ++head;
    }
    iterator mergeSort(iterator lo, iterator hi, std::size_t sz) {
        if (sz <= 1) return lo;
        auto mi { lo + sz / 2 };
        lo = mergeSort(lo, mi, sz / 2);
        mi = mergeSort(mi, hi, sz - sz / 2);
        return merge(lo, mi, hi);
    }
protected:
    void sort(L<T>& l) override {
        mergeSort(l.begin(), l.end(), l.size());
    }
public:
    std::string type_name() const override {
        return "Merge Sort (Downward / by pointer)";
    }
};

}