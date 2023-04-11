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
    using Pos = ListNodePos<T>;
    using Inst = ListNodeInst<T>;
    Inst dummy { Inst::make() };
    Inst& forward(Pos from, size_t step) {
        for (size_t i { 1 }; i < step; ++i) {
            from = from->next();
        }
        return from->next();
    }
    void connect(Pos& prev, Inst& next) {
        next->prev() = prev;
        prev->next() = std::move(next);
        prev = prev->next();
        next = std::move(prev->next());
    }
    void merge(Inst& L1, Inst& L2) {
        Pos L { dummy };
        while (L1 && L2) {
            if (this->cmp(L1->data(), L2->data())) {
                connect(L, L1);
            } else {
                connect(L, L2);
            }
        }
        if (L1) {
            L->next() = std::move(L1);
        } else {
            L->next() = std::move(L2);
        }
        L1 = std::move(dummy->next());
    }
    void mergeSort(Inst& head, size_t size) {
        if (size < 2) return;
        Inst mi { std::move(forward(head, size / 2)) };
        mergeSort(head, size / 2);
        mergeSort(mi, size - size / 2);
        merge(head, mi);
    }
protected:
    void sort(List<T>& L) override {
        if (L.empty()) {
            return;
        }
        Inst first { std::move(L.head()->next()) };
        Inst tail { std::move(L.tail()->prev()->next()) };
        mergeSort(first, L.size());
        forward(first, L.size() - 1)->next() = std::move(tail);
        L.head()->next() = std::move(first);
    }
};

}

