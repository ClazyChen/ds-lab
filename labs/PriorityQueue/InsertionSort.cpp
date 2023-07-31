import Vector;
import Framework;
import Sort.AbstractSort;
import Sort.InsertionSort;
import Sort.SelectionSort;
import Sort.MergeSort;
import std;

using namespace dslab;
using namespace std;

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractVector<T>, Linear<T>>
class BinaryInsertionSort : public AbstractSort<T, Linear> {
protected:
    void sort(Linear<T>& V) override {
        for (Rank i { 1 }; i < V.size(); ++i) {
            if (this->cmp(V[i], V[i - 1])) {
                auto pos { upper_bound(begin(V), begin(V) + i, V[i], this->cmp) };
                auto tmp { move(V[i]) };
                move_backward(pos, begin(V) + i, begin(V) + i + 1);
                *pos = move(tmp);
            }
        }
    }
public:
    string type_name() const override {
        return "Insertion Sort / Binary";
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
    SortTesterImpl<int, InsertionSort>,
    SortTesterImpl<int, BinaryInsertionSort>,
    SortTesterImpl<int, MergeSort>
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
    /*cout << "Experiment 2 : Limited Number in [0, 10)" << endl;
    runTests([](auto& V) {
        iota(begin(V), end(V), 0);
        transform(begin(V), end(V), begin(V), [](auto x) { return x % 10; });
    });*/
}