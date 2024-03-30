#include "sort.hpp"
#include "vector.hpp"
#include <numeric>

using namespace dslab;

template <typename T, template<typename> typename L>
class MergeSortLimit : public MergeSort<T, L> {
protected:
    using MergeSort<T, L>::mergeSort;
    using MergeSort<T, L>::cmp;
    void sort(L<T>& V) override {
        auto mi { --V.end() };
        while (mi != V.begin() && cmp(*(mi - 1), *mi)) {
            --mi;
        }
        if (mi == V.begin()) return;
        auto max_left { *std::max_element(V.begin(), mi) };
        auto left { std::lower_bound(mi, V.end(), max_left, cmp) };
        mergeSort(V.begin(), left, std::distance(V.begin(), left));
    }
public:
    std::string type_name() const override {
        return "Merge Sort (Downward with limit)";
    }
};

template <typename T, template<typename> typename L>
class MergeSortCond : public AbstractSort<T, L> {
protected:
    using iterator = typename L<T>::iterator;
    using AbstractSort<T, L>::cmp;
    void merge(iterator lo, iterator mi, iterator hi, std::size_t ls) {
        static Vector<T> W {};
        if (cmp(*(mi - 1), *mi)) return;
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
        return "Merge Sort (Downward with condition)";
    }
};

template <typename T, template<typename> typename L>
class MergeSortInplace : public AbstractSort<T, L> {
protected:
    using iterator = typename L<T>::iterator;
    using AbstractSort<T, L>::cmp;
    Vector<T> W;
    void merge(iterator lo, iterator mi, iterator hi, iterator tmp) {
        std::swap_ranges(lo, mi, tmp);
        auto i { tmp }, j { mi }, k { lo }, te { tmp + std::distance(lo, mi) };
        while (i != te && j != hi) {
            if (cmp(*j, *i)) {
                std::iter_swap(k++, j++);
            } else {
                std::iter_swap(k++, i++);
            }
        }
        while (i != te) {
            std::iter_swap(k++, i++);
        }
        while (j != hi) {
            std::iter_swap(k++, j++);
        }
    }
    void mergesort(iterator lo, iterator hi) {
    	if (std::distance(lo, hi) < 2) return;
        auto mi2 { hi - std::distance(lo, hi) / 2 };
        mergesort(mi2, hi);
        while (std::distance(lo, mi2) > 1) {
            auto mi1 { mi2 - std::distance(lo, mi2) / 2 };
            mergesort(mi1, mi2);
            merge(mi1, mi2, hi, lo);
            mi2 = mi1;
        }
        merge(lo, mi2, hi, W.begin());
    }
    void sort(L<T>& V) override {
    	W.resize(1);
        mergesort(V.begin(), V.end());
    }
public:
    std::string type_name() const override {
        return "Merge Sort (In-place)";
    }
};

template <typename T>
class MergeSortTest : public Algorithm<bool()> {
public:
    virtual void initialize(const Vector<T>& V) = 0;
};

template <typename T, template <typename, template<typename> typename> typename Sort>
    requires std::is_base_of_v<AbstractSort<T, DefaultVector>, Sort<T, DefaultVector>>
class MergeSortTestImpl : public MergeSortTest<T> {
    Vector<T> V;
    Sort<T, DefaultVector> sorter;

public:
    void initialize(const Vector<T>& V) override {
        this->V = V;
    }

    bool operator()() override {
        sorter(V);
        return std::is_sorted(V.begin(), V.end());
    }

    std::string type_name() const override {
        return sorter.type_name();
    }
};

constexpr std::size_t N { 1000000 };

std::vector<std::pair<std::size_t, std::size_t>> testCases {
    {0, 10 },
    {0, 100},
    {0, 1000},
    {0, 10000},
    {0, 100000},
    {0, 1000000},
    {999990, 1000000},
    {999900, 1000000},
    {999000, 1000000},
    {990000, 1000000},
    {900000, 1000000}
};

TestFramework<MergeSortTest<int>,
    MergeSortTestImpl<int, MergeSort>,
    MergeSortTestImpl<int, MergeSortUpward>,
    MergeSortTestImpl<int, MergeSortLimit>,
    MergeSortTestImpl<int, MergeSortInplace>,
    MergeSortTestImpl<int, MergeSortCond>
	> test;

int main() {
    for (Vector<int> V(N); auto [lo, hi] : testCases) {
        std::cout << std::format("Testing range [{:>6}, {:>6})", lo, hi) << std::endl;
        std::iota(V.begin(), V.end(), 0);
        std::shuffle(V.begin() + lo, V.begin() + hi, Random::engine());
        test.run([&V](auto& test) { test.initialize(V); });
        test();
    }
    return 0;
}