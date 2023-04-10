module;
#include <memory>
#include <algorithm>
#include <type_traits>
#include <iostream>

export module Sort.MergeSort:ListMergeSort;

import Vector;
import List.AbstractList;
import List.ListNode;
import Sort.AbstractSort;

export namespace dslab {

template <typename T, template<typename> typename List>
    requires std::is_base_of_v<AbstractList<T>, List<T>>
class ListMergeSort : public AbstractSort<T, List> {
    using Ptr = std::unique_ptr<ListNode<T>>;
    bool cmp(const T& lhs, const T& rhs) {
        return AbstractSort<T, List>::cmp(lhs, rhs);
    }
    void connect(auto& prev, auto& next) {
        next->prev() = prev;
        prev->next() = std::move(next);
    }
    void merge(ListNode<T>* prev, Ptr lo, Ptr mi, Ptr hi) {
        Ptr i { std::move(lo) }, j { std::move(mi) };
        ListNode<T>* k { prev };
        while (i || j) {
            if (i == nullptr || j && cmp(j->data(), i->data())) {
                connect(k, j);
                j = std::move(k->next()->next());
            } else {
                connect(k, i);
                i = std::move(k->next()->next());
            }
            k = k->next().get();
        }
        connect(k, hi);
    }
    Ptr& forward(ListNode<T>* from, size_t step) {
        for (size_t i { 1 }; i < step; ++i) {
            from = from->next().get();
        }
        return from->next();
    }
    void mergeSort(ListNode<T>* head, Ptr tail, size_t size) {
        if (size < 2) {
            forward(head, size + 1) = std::move(tail);
            return;
        }
        mergeSort(head, std::move(forward(head, size / 2 + 1)), size / 2, cmp);
        mergeSort(forward(head, size / 2).get(), std::move(tail), size - size / 2, cmp);
        auto lo { std::move(forward(head, 1)) };
        auto mi { std::move(forward(lo.get(), size / 2)) };
        auto hi { std::move(forward(mi.get(), size - size / 2)) };
        merge(head, std::move(lo), std::move(mi), std::move(hi), cmp);
    }
protected:
    void sort(List<T>& L) override {
        mergeSort(L.head(), std::move(forward(L.head(), L.size())->next()), L.size());
    }
};

}

