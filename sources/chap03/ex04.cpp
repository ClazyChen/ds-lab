#include "list_sort.h"
#include "vector_sort.h"
#include "list.h"
#include "list_forward.h"
#include "vector.h"
#include "test_framework.h"
using namespace clazy_framework;

// 承接上个例子，这个例子说明，归并排序的效率和数据的初始性质相关

// 在归并排序里，两个指针从左到右移动的过程中
// 向量的左半部分和右半部分是地位平等的，而列表不是
// 列表左指针右移的成本，远远小于右指针右移
// 所以，如果列表天生有序，那么列表的归并排序中不需要进行任何赋值操作，向量则需要不断搬到辅助存储空间去
// 所以最好情况下列表的效率能提升很多，反过来，如果列表天生是倒序的，则会表现很差

// 在这个例子中，实验框架的主体部分和上个例子是一样的

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
};

template <typename Container>
requires (is_data_structure<int, Container>)
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

    void clear() override {
        C.clear();
    }
};

enum class DataType {
    Increasing, Decreasing
};

// enum class 转换为 string，似乎没有非常好的方法
// C++23据说会引入反射
inline string to_string(DataType dtype) {
    if (dtype == DataType::Increasing) {
        return "Increasing";
    } else /* Decreasing */ {
        return "Decreasing";
    }
}

int testData[] { 10, 1000, 10000, 100'000, 1'000'000, 5'000'000 };

int main() {
    TestFramework<SortProblem, SortTest<ForwardList<int>>, SortTest<BidirectionalList<int>>, SortTest<Vector<int>>> tf;
    for (int n : testData) {
        for (auto dtype : {DataType::Increasing, DataType::Decreasing}) {
            cout << "Testing n = " << n << endl;
            cout << "Testing dtype = " << to_string(dtype) << endl;
            Vector<int> V(n);
            for (int i = 0; i < V.size(); i++) {
                if (dtype == DataType::Increasing) {
                    V[i] = i;
                } else /* Decreasing */ {
                    V[i] = V.size() - i - 1;
                }
            }
            tf.invoke(bind(&SortProblem::initialize, placeholders::_1, V));
            tf.test();
            tf.clear();
        }
    }
    return 0;
}

// 实验结果表明，无论是单向还是双向的列表，正序条件和倒序条件下的效率都有显著的差异
// 而向量在这两种情况下的效率差异不大