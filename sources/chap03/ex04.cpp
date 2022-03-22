#include "list_sort.h"
#include "vector_sort.h"
using namespace clazy_framework;

// 承接上个例子，这个例子说明，归并排序的效率和数据的初始性质相关

// 在归并排序里，两个指针从左到右移动的过程中
// 向量的左半部分和右半部分是地位平等的，而列表不是
// 列表左指针右移的成本，远远小于右指针右移
// 所以，如果列表天生有序，那么列表的归并排序中不需要进行任何赋值操作，向量则需要不断搬到辅助存储空间去
// 所以最好情况下列表的效率能提升很多，反过来，如果列表天生是倒序的，则会表现很差
// 注意，向量由于在顺序的情况下，后半部分不需要显式归并，所以向量在顺序的情况下，效率也高于倒序
// 只是列表更加明显

// 单向链表和双向链表
template <typename T>
using ForwardList = clazy::ForwardList<T>;

template <typename T>
using BidirectionalList = clazy::List<T>;

template <typename T>
using Vector = clazy::Vector<T>;

// 各自的排序算法
template <typename T>
using ForwardListMergeSort = clazy::ListMergeSort<T, clazy::ListNodePos<T>, clazy::ForwardListNode<T>>;

template <typename T>
using BidirectionalListMergeSort = clazy::ListMergeSort<T>;

template <typename T>
using VectorMergeSort = clazy::VectorMergeSort<T, Vector<T>>;

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

// 排序和查找算法
template <typename T>
class SortProblem : public Algorithm {
public:
    virtual void init(int n, DataType dtype) = 0; // 初始化一个规模为n的数据结构
    virtual void applySort() = 0; // 进行排序操作
};

template <typename Container, typename SortMethod>
requires (is_base_of_v<clazy_framework::AbstractLinearStructure<int>, Container> && 
          is_base_of_v<clazy_framework::Sort<int, Container>, SortMethod>)
class SortInstance : public SortProblem<int> {
protected:
    Container C; // 用来进行排序的数据结构
    SortMethod sortAlgorithm;
public:
    virtual void init(int n, DataType dtype) {
        C.clear();
        if (dtype == DataType::Increasing) {
            for (int i : views::iota(0, n)) {
                C.push_back(i);
            }
        } else /* dtype == DataType::Decreasing */ {
            for (int i : views::iota(0, n) | views::reverse) {
                C.push_back(i);
            }
        }
    }
    virtual void applySort() {
        sortAlgorithm.apply(C);
    }
    virtual string getTypename() const { // 重载typename，短路到数据结构名上
        return typeid(C).name();
    }
};

int main() {
    auto algorithms = generateInstances<
        SortProblem<int>, 
        SortInstance<ForwardList<int>, ForwardListMergeSort<int>>,
        SortInstance<BidirectionalList<int>, BidirectionalListMergeSort<int>>,
        SortInstance<Vector<int>, VectorMergeSort<int>>>();
    int testData[] { 10, 1000, 10000, 100'000, 1'000'000, 5'000'000 };
    for (int n : testData) {
        for (auto dtype : {DataType::Increasing, DataType::Decreasing}) {
            cout << "Testing n = " << n << endl;
            cout << "Testing dtype = " << to_string(dtype) << endl;
            for (auto algorithm : algorithms) {
                algorithm->init(n, dtype);
            }
            applyTest<SortProblem<int>>(algorithms, [&](auto algorithm) {
                algorithm->applySort();
            });
        }
    }
    return 0;
}

// 具体而言，您还可以根据实验结果观察到，双向链表相比于单向链表，受到的影响更大
// 因为双向链表在右侧指针移动的时候，需要对链子做6次赋值