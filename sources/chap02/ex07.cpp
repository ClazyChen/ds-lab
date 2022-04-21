#include "vector.h"
#include "vector_sort.h"
#include "random.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子研究向量唯一化的问题
// 输入：向量
// 输出：删去向量中的重复元素，剩余元素的相对位置保持不变（重复元素保留第一个）
//      返回被删除的元素数量

template <typename T>
using Vector = clazy::Vector<T>;

template <typename T>
using SortAlgorithm = clazy::VectorMergeSort<T, Vector<T>>;

// 唯一化问题
template <typename T>
class DeduplicateProblem : public Algorithm<int, Vector<T>&> {

};

// 直接唯一化方法
// 对于每个元素，如果在之前出现过，则将其删除
template <typename T>
class DirectDeduplicate : public DeduplicateProblem<T> {
public:
    virtual int apply(Vector<T>& V) override {
        int oldSize = V.size();
        for (Rank r = 1; r < V.size(); ) {
            if (any_of(V.begin(), V.begin() + r, [&](auto& x) { return x == V[r]; })) {
                V.remove(r); // r已经被删掉了，所以不需要向后移动
            } else {
                r++;
            }
        }
        return oldSize - V.size();
    }
};

// 间接唯一化方法，需要可排序
// 首先将向量排序，然后利用有序向量中重复元素必定相邻的特点，降低计算量（类似于GFGR的批量删除）
// 最后将向量还原为排序前的相对次序
// 需要额外的空间，但在排序性能较高时，有机会降低时间

// 首先是已排序向量的唯一化，使用快慢指针实现
// 当且仅当一个元素和它前面的元素相同，它应当被删掉
template <typename T>
class SortedDeduplicate : public DeduplicateProblem<T> {
public:
    virtual int apply(Vector<T>& V) override {
        auto it_assign = V.begin();         // 赋值指针（慢指针）
        for (auto it_find = V.begin(); it_find != V.end(); it_find++) {   // 查找指针（快指针）
            if (it_find == V.begin() || !(*it_find == *(it_find - 1))) { // 如果是不会被删除的元素
                *(it_assign++) = *it_find; // 则将其赋值到赋值指针的位置上
            }                              // 否则将其丢弃，赋值指针不移动
        }
        int removed = V.end() - it_assign;  // 快慢指针的距离就是被删除的元素数量
        V.resize(it_assign - V.begin());    // 修改V的规模，丢弃后面的元素
        return removed;
    }
};

// 利用上面的有序向量唯一化进行包装
// 通过(data, r)进行包装，其中data是原先的元素的值，r是元素在原向量中的秩
template <typename T>
class SortBasedDeduplicate : public DeduplicateProblem<T> {
protected:
    struct E {
        T data;
        Rank r;
        bool operator==(const E& other) const {
            return data == other.data;
        }
        auto operator<=>(const E& other) const {
            return data <=> other.data;
        }
    }; // 采用元素+原向量中的秩作为次序
    SortAlgorithm<E> sortAlgorithm;
    SortedDeduplicate<E> sortedDeduplicate;
public:
    virtual int apply(Vector<T>& V) override {
        Vector<E> VE(V.size());                // 建立辅助向量
        for (Rank r = 0; r < V.size(); r++) {  // 将原向量中的元素转换为辅助向量中的元素
            VE[r] = {V[r], r};
        }
        sortAlgorithm(VE);                     // 对辅助向量进行排序，这里需要保证排序是稳定的
        int removed = sortedDeduplicate(VE);   // 调用有序向量的唯一化
        sortAlgorithm(VE, [](auto& e1, auto& e2) {
            return e1.r <= e2.r;
        });                                    // 重新排序，复原相对顺序
        V.resize(VE.size());
        for (Rank r = 0; r < V.size(); r++) {  // 将辅助向量中的元素转换回原向量中的元素
            V[r] = VE[r].data;
        }
        return removed;
    }
};

// 为了让元素有比较多的重复，这里规定一个ratio，选取数字的区间是规模乘上这个比例
const double compress_ratio = 0.5;
int rangeLimit(int n) {
    return n * compress_ratio;
}

int testData[] { 10, 100, 1000, 10000, 100'000 };

int main() {
    TestFramework<DeduplicateProblem<int>, DirectDeduplicate<int>, SortBasedDeduplicate<int>> tf;
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        auto V = clazy::RandomVector<int, Vector<int>>()(n, 0, rangeLimit(n));
        tf.test(V);
    }
    return 0;
}

