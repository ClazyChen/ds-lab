#include "list_sort.h"
#include "vector_sort.h"
#include "list.h"
#include "list_forward.h"
#include "vector.h"
#include "random.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子将会讨论列表的排序
// 同样，我们拿单向链表、双向链表和作为参考对象的向量来对比
template <typename T>
using ForwardList = clazy::ForwardList<T>;

template <typename T>
using BidirectionalList = clazy::List<T>;

template <typename T>
using Vector = clazy::Vector<T>;

// 各自的排序算法
template <typename T>
using ForwardListMergeSort = clazy::ForwardListMergeSort<T, ForwardList<T>>;

template <typename T>
using BidirectionalListMergeSort = clazy::ListMergeSort<T, BidirectionalList<T>>;

template <typename T>
using VectorMergeSort = clazy::VectorMergeSort<T, Vector<T>>;

// 初始化阶段，采用RandomVector生成一个随机的线性表
// 然后进行排序测试，并验证正确性
// 为了保证公平，输出的向量是统一的
class SortProblem : public Algorithm<> {
public:
    virtual void initialize(const Vector<int>& V) = 0;
    virtual void check() const = 0;
};

template <typename Container>
requires (is_linear_structure<int, typename Container::Position, Container>)
class SortTest : public SortProblem {
private:
    Container C;
public:
    void initialize(const Vector<int>& V) override {
        for (int x : V) {
            C.push_back(x);
        }
    }

    void apply() override {
        if constexpr (is_same_v<Vector<int>, Container>) {
            VectorMergeSort<int> sorter;
            sorter(C);
        } else if constexpr (is_same_v<BidirectionalList<int>, Container>) {
            BidirectionalListMergeSort<int> sorter;
            sorter(C);
        } else if constexpr (is_same_v<ForwardList<int>, Container>) {
            ForwardListMergeSort<int> sorter;
            sorter(C);
        }
    }

    void check() const override {
        assert(is_sorted(begin(C), end(C)));
    }

    void clear() override {
        C.clear();
    }
};

int testData[] { 10, 1000, 10000, 100'000, 1'000'000, 5'000'000 };

int main() {
    TestFramework<SortProblem, SortTest<ForwardList<int>>, SortTest<BidirectionalList<int>>, SortTest<Vector<int>>> tf;
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        auto V = clazy::RandomVector<int, Vector<int>>()(n);
        tf.invoke(bind(&SortProblem::initialize, placeholders::_1, V));
        tf.test();
        tf.invoke(bind(&SortProblem::check, placeholders::_1));
        tf.clear();
    }
    return 0;
}

// 实验结果应该是，列表的归并排序的效率会低于向量，因为需要更多次数的赋值（平均）
// 单向链表比双向链表快一些，因为需要的赋值次数显著减少了
// 在列表归并的过程中，会破坏掉局部性，所以当n增大时，局部性的缺失会进一步放大列表和向量的性能差距
