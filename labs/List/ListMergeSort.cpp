import Framework;
import Vector;
import List;
import LinearList;
import Sort;
import std;

using namespace dslab;
using namespace std;

class SortProblem : public Algorithm<bool()> {
public:
    virtual void initialize(const Vector<int>& V0) = 0;
    virtual void shuffle(const Vector<int>& Vi) = 0;
};

template <template<typename> typename Linear, template<typename, template<typename> typename> typename SortAlgo>
    requires std::is_base_of_v<AbstractLinearList<int, typename Linear<int>::position_type>, Linear<int>>&& std::is_base_of_v<AbstractSort<int, Linear>, SortAlgo<int, Linear>>
class SortImpl : public SortProblem {
    Linear<int> L;
    SortAlgo<int, Linear> sort;
public:
    void initialize(const Vector<int>& V0) override {
        L.clear();
        for (auto e : V0) {
            L.push_back(e);
        }
    }
    void shuffle(const Vector<int>& Vi) override {
        if constexpr (std::is_base_of_v<AbstractList<int>, Linear<int>>) {
            Vector<ListNodeInst<int>> tmp;
            for (size_t i { 0 }; i < Vi.size(); ++i) {
                auto p { move(L.head()->next()) };
                L.head()->next() = move(p->next());
                tmp.push_back(move(p));
            }
            auto p { L.head() };
            auto tail { move(p->next()) };
            for (auto e : Vi) {
                tmp[e]->prev() = p;
                p->next() = move(tmp[e]);
                p = p->next();
            }
            tail->prev() = p;
            p->next() = move(tail);
        } else {
            // Vector, no need to shuffle
        }
    }
    bool operator()() override {
        sort(L);
        return is_sorted(begin(L), end(L));
    }
    string type_name() const override {
        return format("{1} ({0})", L.type_name(), sort.type_name());
    }
};

vector testData { 10, 1000, 10000, 100'000, 1'000'000 };

TestFramework<SortProblem,
    SortImpl<DefaultVector, MergeSort>,
    SortImpl<DefaultVector, MergeSortUpward>,
    SortImpl<List, MergeSort>,
    SortImpl<List, MergeSortUpward>,
    SortImpl<List, ListMergeSort>
> test;

default_random_engine engine { random_device{}() };

int main() {
    for (auto n : testData) {
        cout << format("n = {:>10}", n) << endl;
        Vector<int> V(n);
        iota(begin(V), end(V), 0);
        shuffle(begin(V), end(V), engine);
        test.run([V](auto& algo) { algo.initialize(V); });
        shuffle(begin(V), end(V), engine);
        test.run([V](auto& algo) { algo.shuffle(V); });
        test();
    }
}
