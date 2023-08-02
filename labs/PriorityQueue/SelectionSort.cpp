import Vector;
import Framework;
import Sort.AbstractSort;
import Sort.SelectionSort;
import std;

using namespace dslab;
using namespace std;

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractVector<T>, Linear<T>>
class DoubleSelectionSort : public AbstractSort<T, Linear> {
protected:
    void sort(Linear<T>& V) override {
        Rank lo { 0 }, hi { V.size() - 1 };
        while (lo < hi) {
            Rank j { lo }, k { lo };
            for (Rank l { lo + 1 }; l <= hi; ++l) {
                if (this->cmp(V[l], V[j])) {
                    j = l;
                }
                if (this->cmp(V[k], V[l])) {
                    k = l;
                }
            }
            if (k == lo) {
                k = j;
            }
            swap(V[lo++], V[j]);
            swap(V[hi--], V[k]);
        }
    }
public:
    string type_name() const override {
        return "Double Selection Sort";
    }
};

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractVector<T>, Linear<T>>
class BingoSort : public AbstractSort<T, Linear> {
protected:
    void sort(Linear<T>& V) override {
        for (Rank i { 0 }; i < V.size(); ) {
            auto min { *min_element(begin(V) + i, end(V), this->cmp) };
            for (Rank j { i }; j < V.size(); ++j) {
                if (V[j] == min) {
                    swap(V[i++], V[j]);
                }
            }
        }
    }
public:
    string type_name() const override {
        return "Bingo Sort";
    }
};

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractVector<T>, Linear<T>>
class StableSelectionSort : public AbstractSort<T, Linear> {
protected:
    void sort(Linear<T>& V) override {
        for (auto i { begin(V) }; i != end(V); ++i) {
            if (auto j { min_element(i, end(V), this->cmp) }; j != i) {
                auto tmp { move(*j) };
                move_backward(i, j, j + 1);
                *i = move(tmp);
            }
        }
    }
public:
    string type_name() const override {
        return "Selection Sort (Stable Ver.)";
    }
};

template <typename T>
class SortTester : public Algorithm<bool()> {
protected:
    Vector<T> V;
public:
    void initialize(const Vector<T>& V) {
        this->V = V;
    }
};

template <typename T, template <typename, template<typename> typename> typename Sort>
class SortTesterImpl : public SortTester<T> {
    Sort<T, DefaultVector> sort;
public:
    bool operator()() override {
        sort(this->V);
        return is_sorted(begin(this->V), end(this->V));
    }

    string type_name() const override {
        return sort.type_name();
    }
};

default_random_engine engine { random_device{}() };
vector testData { 10, 100, 1000, 3000, 10000 };

TestFramework<SortTester<int>,
    SortTesterImpl<int, SelectionSort>,
    SortTesterImpl<int, DoubleSelectionSort>,
    SortTesterImpl<int, StableSelectionSort>,
    SortTesterImpl<int, BingoSort>
> test;

void runTests(auto&& generator) {
    for (auto n : testData) {
        cout << format("n = {:>8}", n) << endl;
        Vector<int> V(n);
        generator(V);
        shuffle(begin(V), end(V), engine);
        test.run([&V](auto& algo) { algo.initialize(V); });
        test();
    }
}

int main() {
    cout << "Experiment 1 : Random Number" << endl;
    runTests([](auto& V) { iota(begin(V), end(V), 0); });
    cout << "Experiment 2 : Limited Number in [0, 10)" << endl;
    runTests([](auto& V) {
        iota(begin(V), end(V), 0);
        transform(begin(V), end(V), begin(V), [](auto x) { return x % 10; });
    });
}