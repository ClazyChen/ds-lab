#include "list_sort.h"
#include "vector_sort.h"
#include "list_search.h"
#include "vector_search.h"
#include "random.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子将会讨论列表的排序和查找
// 同样，我们拿单向链表、双向链表和向量来对比
template <typename T>
using ForwardList = clazy::List<T, clazy::ForwardListNode<T>>;

template <typename T>
using BidirectionalList = clazy::List<T>;

template <typename T>
using Vector = clazy::Vector<T>;

// 各自的排序算法
template <typename T>
using ForwardListMergeSort = clazy::ListMergeSort<T, clazy::ForwardListNode<T>, ForwardList<T>>;

template <typename T>
using BidirectionalListMergeSort = clazy::ListMergeSort<T, clazy::ListNode<T>, BidirectionalList<T>>;

template <typename T>
using VectorMergeSort = clazy::VectorMergeSort<T, Vector<T>>;

// 各自的查找算法，其中向量采用折半查找
template <typename T>
using ForwardListSearch = clazy::ListSequentialSearch<T, clazy::ForwardListNode<T>, ForwardList<T>>;

template <typename T>
using BidirectionalListSearch = clazy::ListSequentialSearch<T, clazy::ListNode<T>, BidirectionalList<T>>;

template <typename T>
using VectorSearch = clazy::VectorBinarySearch<T, Vector<T>>;

// 排序和查找算法
template <typename T>
class SortSearchProblem : public Algorithm {
public:
    virtual void init(const clazy::Vector<T>& V) = 0; // 用一个向量，初始化一个规模为n的数据结构
    virtual void applySort() = 0; // 进行排序操作
    virtual void checkSort() = 0; // 检查排序结果
    virtual void applySearch(const clazy::Vector<T>& V) = 0; // 进行查找操作
    virtual void checkSearch(const clazy::Vector<T>& V) = 0; // 测试查找结果
};

template <typename T, typename Container, typename SortMethod, typename SearchMethod, typename P>
requires (is_base_of_v<clazy_framework::AbstractLinearStructure<T>, Container> && 
          is_base_of_v<clazy_framework::Sort<T, Container>, SortMethod> && 
          is_base_of_v<clazy_framework::OrderedSearch<T, Container, P>, SearchMethod>)
class SortSearchInstance : public SortSearchProblem<T> {
protected:
    Container C; // 用来进行排序的数据结构
    SortMethod sortAlgorithm;
    SearchMethod searchAlgorithm;
public:
    virtual void init(const clazy::Vector<T>& V) {
        C.clear();
        for (auto x : V) {
            C.push_back(x);
        }
    }
    virtual void applySort() {
        sortAlgorithm.apply(C);
    }
    virtual void applySearch(const clazy::Vector<T>& V) {
        if constexpr (is_base_of_v<AbstractList<T>, Container>) {
            if ((long long)C.size() * V.size() >= 10'000'000'000LL) {
                cout << "(no action)"; return; // 如果查找次数太多，把列表的顺序查找屏蔽掉
            }
        }
        for (auto x : V) {
            searchAlgorithm.apply(C, x);
        }
    }
    virtual void checkSort() {
        assert(is_sorted(begin(C), end(C)));
    }
    virtual void checkSearch(const clazy::Vector<T>& V) {
        C.clear();
        for (auto x : V) {
            auto [r, p] = searchAlgorithm.apply(C, x);
            if constexpr (is_base_of_v<AbstractVector<T>, Container>) {
                if (r) {
                    assert(C[p] == x);
                } else {
                    C.insert(p, x);
                }
            } else {
                if (r) {
                    assert(p->data() == x);
                } else {
                    C.insertAsPred(p, x);
                }
            }
        }
        checkSort();
    }
    virtual string getTypename() const { // 重载typename，短路到数据结构名上
        return typeid(C).name();
    }
};

const int search_count = 100'000; // 查找操作的次数
const int test_search_count = 100; // 进行测试查找的次数

int main() {
    auto algorithms = generateInstances<
        SortSearchProblem<int>, 
        SortSearchInstance<int, ForwardList<int>, ForwardListMergeSort<int>, ForwardListSearch<int>, ListNodePos<int>>,
        SortSearchInstance<int, BidirectionalList<int>, BidirectionalListMergeSort<int>, BidirectionalListSearch<int>, ListNodePos<int>>,
        SortSearchInstance<int, Vector<int>, VectorMergeSort<int>, VectorSearch<int>, Rank>>();
    int testData[] { 10, 1000, 10000, 100'000, 1'000'000 };
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        auto data = randomVector(n);
        auto sdata = randomVector(search_count);
        auto searchTestData = randomVector(test_search_count);
        for (auto algorithm : algorithms) {
            algorithm->checkSearch(searchTestData);
        }
        for (auto algorithm : algorithms) {
            algorithm->init(data);
        }
        cout << "Applying Sort" << endl;
        applyTest<SortSearchProblem<int>>(algorithms, [](auto algorithm) {
            algorithm->applySort();
        });
        for (auto algorithm : algorithms) {
            algorithm->checkSort();
        }
        cout << "Applying Search" << endl;
        applyTest<SortSearchProblem<int>>(algorithms, [&sdata](auto algorithm) {
            algorithm->applySearch(sdata);
        });
    }
    return 0;
}

// 如前所述，这里测试的时候列表中的节点也是有高度的局部性的
// 如果列表中的节点的地址是乱序的，会和上个实验一样，排序性能更加降低
// 至于查找，由于列表不能提供随机访问，只能顺序查找，所以无论如何都会比向量性能低