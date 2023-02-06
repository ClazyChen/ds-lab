#include <vector>
#include <format>
#include <numeric>
#include <random>
#include <iostream>
import Framework;
import Vector;
import List;
import LinearList;
import Sort;
using namespace dslab;
using namespace std;

class SortProblem : public Algorithm<bool> {
public:
    virtual void initialize(const Vector<int>& V0) = 0;
};

template <template<typename> typename Linear, template<typename, template<typename> typename, typename> typename SortAlgo, typename Comparator = std::less<int>>
    requires std::is_base_of_v<AbstractLinearList<int, typename Linear<int>::position_type>, Linear<int>>&& std::is_base_of_v<AbstractSort<int, Linear, Comparator>, SortAlgo<int, Linear, Comparator>>
class SortImpl : public SortProblem {
    Linear<int> L;
    SortAlgo<int, Linear, Comparator> sort;
public:
    void initialize(const Vector<int>& V0) override {
        L.clear();
        for (auto e : V0) {
            L.push_back(e);
        }
    }
    bool operator()() override {
        sort(L);
        return is_sorted(begin(L), end(L));
    }
};

// 重载type_name以优化输出
class VectorSort : public SortImpl<DefaultVector, MergeSort> {
public:
    static string type_name() {
        return "MergeSort(Vector)";
    }
};

class ListSort : public SortImpl<List, MergeSort> {
public:
    static string type_name() {
        return "MergeSort(List Val)";
    }
};

class ListSortPtr : public SortImpl<List, ListMergeSort> {
public:
    static string type_name() {
        return "MergeSort(List Ptr)";
    }
};

vector testData { 10, 1000, 10000, 100'000, 1'000'000 };

TestFramework<SortProblem, VectorSort, ListSort, ListSortPtr> test;
default_random_engine engine;

int main() {
    for (auto n : testData) {
        cout << format("n = {:10d}", n) << endl;
        Vector<int> V(n);
        iota(begin(V), end(V), 0);
        shuffle(begin(V), end(V), engine);
        test.run([V](auto& algo) { algo.initialize(V); });
        test();
    }
}
