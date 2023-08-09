import Vector;
import Framework;
import Sort.AbstractSort;
import Sort.InsertionSort;
import Sort.SelectionSort;
import Sort.MergeSort;
import Sort.TournamentSort;
import Sort.HeapSort;
import std;

using namespace dslab;
using namespace std;

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
    SortTesterImpl<int, InsertionSort>,
    SortTesterImpl<int, BinaryInsertionSort>,
    SortTesterImpl<int, MergeSort>,
    SortTesterImpl<int, TournamentSort>,
    SortTesterImpl<int, HeapSort>
> test;

void runTests(auto&& shuffle_ratio) {
    for (auto n : testData) {
        cout << format("n = {:>8}", n) << endl;
        Vector<int> V(n);
        iota(begin(V), end(V), 0);
        shuffle(begin(V), begin(V) + static_cast<int>(shuffle_ratio(n)), engine);
        test.run([&V](auto& algo) { algo.initialize(V); });
        test();
    }
}

int main() {
    cout << "Experiment 1 : All n Elements     are Shuffled" << endl;
    runTests([](int n) { return n; });
    cout << "Experiment 2 : Only First 0.1 * n are Shuffled" << endl;
    runTests([](int n) { return n * 0.1; });
    cout << "Experiment 3 : Only First logn    are Sorted" << endl;
    runTests([](int n) { return log2(n); });
}