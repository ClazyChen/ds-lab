#include "sort.hpp"
#include "vector.hpp"
#include "list.hpp"

using namespace dslab;

template <typename T>
class SortProblem : public Algorithm<bool()> {
public:
    virtual void initialize(const Vector<T>& V0) = 0;
    virtual void shuffle(const Vector<T>& Vi) = 0;
};

template <
    typename T,
    template<typename> typename L,
    template<typename, template<typename> typename> typename SortAlgo
> requires std::is_base_of_v<LinearList<T, typename L<T>::iterator, typename L<T>::const_iterator>, L<T>> && std::is_base_of_v<AbstractSort<T, L>, SortAlgo<T, L>>
class SortImpl : public SortProblem<T> {
    L<T> V;
    SortAlgo<T, L> sort;
public:
    void initialize(const Vector<T>& V0) override {
        V.clear();
        for (auto e : V0) {
            V.push_back(e);
        }
    }
    void shuffle(const Vector<T>& Vi) override {
        if constexpr (std::is_base_of_v<AbstractList<T>, L<T>>) {
            std::vector<std::unique_ptr<ListNode<T>>> tmp;
            auto last { --V.begin() }; 
			while (last + 1 != V.end()) {
                auto p { std::move(last.node()->next()) };
                last.node()->next() = std::move(p->next());
                tmp.push_back(std::move(p));
            }
            auto tail { std::move(last.node()->next()) };
            for (auto e : Vi) {
                tmp[e]->prev() = last.node();
                last.node()->next() = std::move(tmp[e]);
                ++last;
            }
            tail->prev() = last.node();
            last.node()->next() = std::move(tail);
        } else {
            // Vector, no need to shuffle
        }
    }
    bool operator()() override {
        sort(V);
        return std::is_sorted(V.begin(), V.end());
    }
    std::string type_name() const override {
        return format("{1} / {0}", V.type_name(), sort.type_name());
    }
};

std::vector testData { 10, 1000, 10000, 100'000, 1'000'000 };

TestFramework<SortProblem<std::size_t>,
    SortImpl<std::size_t, DefaultVector, MergeSort>,
    SortImpl<std::size_t, DefaultVector, MergeSortUpward>,
    SortImpl<std::size_t, List, MergeSort>,
    SortImpl<std::size_t, List, MergeSortUpward>,
    SortImpl<std::size_t, List, ListMergeSort>
    > test;

template <bool Shuffle>
void testCases() {
    std::cout << std::format("Enabled Shuffle = {}", Shuffle) << std::endl;
    for (auto n : testData) {
        std::cout << std::format("n = {:>10}", n) << std::endl;
        Vector<std::size_t> V(n);
        std::iota(V.begin(), V.end(), 0);
        std::shuffle(V.begin(), V.end(), Random::engine());
        test.run([&V](auto& algo) { algo.initialize(V); });
        if constexpr (Shuffle) {
            std::shuffle(V.begin(), V.end(), Random::engine());
            test.run([&V](auto& algo) { algo.shuffle(V); });
        }
        test();
    }
}

int main() {
    testCases<false>();
    testCases<true>();
    return 0;
}
