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
    Ptr dummy { std::make_unique<ListNode<T>>() };
    auto& forward(ListNode<T>* from, size_t step) {
        for (size_t i { 1 }; i < step; ++i) {
            from = from->next();
        }
        return from->next();
    }
    void connect(auto& prev, auto& next) {
        next->prev() = prev;
        prev->next() = std::move(next);
        prev = prev->next();
        next = std::move(prev->next());
    }
    void merge(auto& L1, auto& L2) {
        auto L { dummy.get() };
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
    }
    void mergeSort(auto& head, size_t size) {
        if (size < 2) return;
        auto mi { std::move(forward(head, size / 2)) };
        mergeSort(head, size / 2);
        mergeSort(mi, size - size / 2);
        merge(head, mi);
        head = std::move(dummy->next());
    }
protected:
    void sort(List<T>& L) override {
        if (L.empty()) {
            return;
        }
        auto first { std::move(L.head()->next()) };
        auto tail { std::move(L.tail()->prev()->next()) };
        mergeSort(first, L.size());
        forward(first, L.size() - 1)->next() = std::move(tail);
        L.head()->next() = std::move(first);
    }
};

}

