#include "list_search.h"
#include "vector_search.h"
#include "sort_merge.h"
#include "list.h"
#include "list_forward.h"
#include "vector.h"
#include "random.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子将会讨论列表的查找
// 我们仍然拿单向链表、双向链表和作为参考对象的向量来对比
template <typename T>
using ForwardList = clazy::ForwardList<T>;

template <typename T>
using BidirectionalList = clazy::List<T>;

template <typename T>
using Vector = clazy::Vector<T>;

// 各自的查找算法，其中向量也采用顺序查找（如果用折半查找太欺负人了）
template <typename T>
using ForwardListSearch = clazy::SequentialSearch<T, ListNodePos<T>, ForwardList<T>>;

template <typename T>
using BidirectionalListSearch = clazy::SequentialSearch<T, ListNodePos<T>, BidirectionalList<T>>;

template <typename T>
using VectorSearch = clazy::SequentialSearch<T, Rank, Vector<T>>;

// 和向量的查找实验类似，固定查找次数，改变被查找的向量的规模
const int random_ratio = 2.0;
int rangeLimit(int n) {
    return (int)(n * random_ratio);
}

const int search_count = 100'000;

class SearchProblem : public Algorithm<void, const Vector<int>&> {
public:
    virtual void initialize(const Vector<int>& V) = 0;
};

template <typename Container>
requires (is_data_structure<int, Container>)
class SearchTest : public SearchProblem {
private:
    Container C;
public:
    void initialize(const Vector<int>& V) override {
        for (int x : V) {
            C.push_back(x);
        }
    }

    void apply(const Vector<int>& V) override {
        if constexpr (is_same_v<Vector<int>, Container>) {
            VectorSearch<int> searcher;
            for (int x : V) {
                searcher(C, x);
            }
        } else if constexpr (is_same_v<BidirectionalList<int>, Container>) {
            BidirectionalListSearch<int> searcher;
            for (int x : V) {
                searcher(C, x);
            }
        } else if constexpr (is_same_v<ForwardList<int>, Container>) {
            ForwardListSearch<int> searcher;
            for (int x : V) {
                searcher(C, x);
            }
        }
    }

    void clear() override {
        C.clear();
    }
};

int testData[] { 10, 100, 1000, 10000 };

int main() {
    TestFramework<SearchProblem, SearchTest<ForwardList<int>>, SearchTest<BidirectionalList<int>>, SearchTest<Vector<int>>> tf;
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        auto V = clazy::RandomVector<int, Vector<int>>()(n, 0, rangeLimit(n));
        clazy::MergeSort<int, decltype(V)>()(V);
        auto search_data = clazy::RandomVector<int, Vector<int>>()(search_count, 0, rangeLimit(n));
        tf.invoke(bind(&SearchProblem::initialize, placeholders::_1, V));
        tf.test(search_data);
    }
}

// 可以看出，向量 < 单向链表 < 双向链表 的关系仍然成立
// 在顺序查找的前提下，三者只有并不夸张的常数级差异
// 但向量是可以折半查找的，所以在查找上列表的性能也完全不能和向量相比