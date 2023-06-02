import Framework;
import Sort;
import Vector;
import std;

using namespace dslab;
using namespace std;

template <typename T, template<typename> typename Linear>
class MergeSortLimit : public MergeSort<T, Linear> {
protected:
    void sort(Linear<T>& L) override {
        Rank mid { L.size() - 1 };
        while (this->cmp(L[mid - 1], L[mid])) {
            if (--mid == 0) return;
        }
        auto max_left { *max_element(begin(L), begin(L) + mid) };
        auto left { lower_bound(begin(L) + mid, end(L), max_left, this->cmp) };
        this->mergeSort(begin(L), left, left - begin(L));
    }
public:
    string type_name() const override {
        return "Merge Sort (Downward with limit)";
    }
};

template <typename T, template<typename> typename Linear>
class MergeSortCond : public AbstractSort<T, Linear> {
    Vector<T> W;
    using Iterator = typename Linear<T>::iterator;
    void merge(Iterator lo, Iterator mi, Iterator hi, size_t size) {
        if (this->cmp(*(mi - 1), *mi)) return;
        W.resize(size / 2);
        std::move(lo, mi, W.begin());
        auto i { W.begin() };
        auto j { mi }, k { lo };
        while (i != W.end() && j != hi) {
            if (this->cmp(*j, *i)) {
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
protected:
    void sort(Linear<T>& L) override {
        mergeSort(std::begin(L), std::end(L), L.size());
    }
public:
    string type_name() const override {
        return "Merge Sort (Downward with judging)";
    }
};

class MergeSortTestProblem : public Algorithm<bool()> {
public:
    virtual void initialize(const Vector<int>& V) = 0;
};

template <template <typename, template<typename> typename> typename Sorter>
    requires is_base_of_v<AbstractSort<int, DefaultVector>, Sorter<int, DefaultVector>>
class MergeSortTest : public MergeSortTestProblem {
    Vector<int> V;
    Sorter<int, DefaultVector> sorter;
public:
    void initialize(const Vector<int>& V) override {
        this->V = V;
    }
    bool operator()() override {
        sorter(V);
        return is_sorted(begin(V), end(V));
    }
    string type_name() const override {
        return sorter.type_name();
    }
};

static constexpr size_t N { 100000 };

vector<pair<Rank, Rank>> testCases {
    {0, 10},
    {0, 100},
    {0, 1000},
    {0, 10000},
    {0, 100000},
    {99990, 100000},
    {99900, 100000},
    {99000, 100000},
    {90000, 100000}
};

TestFramework<MergeSortTestProblem, 
    MergeSortTest<MergeSort>, 
    MergeSortTest<MergeSortUpward>,
    MergeSortTest<MergeSortLimit>,
    MergeSortTest<MergeSortCond>,
    MergeSortTest<TimSort>> test;

default_random_engine engine { random_device{}() };

int main() {
    for (Vector<int> V(N); auto & [lo, hi] : testCases) {
        cout << format("Testing with range [{:>6}, {:>6})\n", lo, hi);
        iota(begin(V), end(V), 0);
        shuffle(begin(V) + lo, begin(V) + hi, engine);
        test.run([&V](auto& test) { test.initialize(V); });
        test();
    }
    return 0;
}