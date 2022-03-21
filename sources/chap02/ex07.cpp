#include "vector.h"
#include "vector_sort.h"
#include "random.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子研究向量唯一化的问题
// 输入：向量
// 输出：删去向量中的重复元素，剩余元素的相对位置保持不变
//      重复元素保留第一个
//      返回被删除的元素数量

template <typename T>
using Vector = clazy::Vector<T>;

// 当您使用自己实现的向量时，相应的排序算法也需要修改
template <typename T>
using SortAlgorithm = clazy::VectorMergeSort<T>;

template <typename T>
class Deduplicate : public Algorithm {
public:
    virtual int apply(Vector<T>& V) = 0;
};

// 直接唯一化方法
// 对于每个元素，如果在之前出现过，则将其删除
template <typename T>
class DirectDeduplicate : public Deduplicate<T> {
public:
    virtual int apply(Vector<T>& V) {
        int oldSize = V.size();
        for (Rank r = 1; r < V.size(); ) {
            if (any_of(begin(V), begin(V) + r, [&](auto& x) { return x == V[r]; })) {
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
class SortedDeduplicate : public Deduplicate<T> {
public:
    virtual int apply(Vector<T>& V) {
        auto it_assign = begin(V);         // 赋值指针（慢指针）
        for (auto it_find = begin(V); it_find != end(V); it_find++) {   // 查找指针（快指针）
            if (it_find == begin(V) || !(*it_find == *(it_find - 1))) { // 如果是不会被删除的元素
                *(it_assign++) = *it_find; // 则将其赋值到赋值指针的位置上
            }                              // 否则将其丢弃，赋值指针不移动
        }
        int removed = end(V) - it_assign;  // 快慢指针的距离就是被删除的元素数量
        V.resize(it_assign - begin(V));    // 修改V的规模，丢弃后面的元素
        return removed;
    }
};

// 利用上面的有序向量唯一化进行包装
template <typename T>
class SortBasedDeduplicate : public Deduplicate<T> {
protected:
    struct E {
        T data;
        Rank r;
        bool operator<=(const E& other) const {
            return data <= other.data;
        }
        bool operator==(const E& other) const {
            return data == other.data;
        }
    }; // 采用元素+原向量中的秩作为次序
    shared_ptr<clazy::VectorSort<E, Vector<E>>> sortAlgorithm;
    shared_ptr<Deduplicate<E>> sortedDeduplicate;
public:
    SortBasedDeduplicate(): sortAlgorithm(make_shared<SortAlgorithm<E>>()), sortedDeduplicate(make_shared<SortedDeduplicate<E>>()) {}
    virtual int apply(Vector<T>& V) {
        Vector<E> VE(V.size());                // 建立辅助向量
        for (int i : views::iota(0, V.size())) {
            VE.push_back({V[i], i});           // 记录每个元素的秩
        }
        sortAlgorithm->apply(VE, [](auto& e1, auto& e2) {
            if (e1.data <= e2.data) {
                return e1.data == e2.data ? e1.r <= e2.r : true;
            } else {
                return false;
            }
        });                                    // 双关键字排序，这样可以使用非稳定的排序
        int removed = sortedDeduplicate->apply(VE);
        sortAlgorithm->apply(VE, [](auto& e1, auto& e2) {
            return e1.r <= e2.r;
        });                                    // 重新排序，复原相对顺序
        V.resize(VE.size());
        transform(begin(VE), end(VE), begin(V), [](auto& e) {
            return e.data;
        });                                    // 将辅助向量中的元素转移回原向量
        return removed;
    }
};

// 判断是否唯一化的函数
template <typename T>
bool checkDeduplicated(const Vector<T>& V) {
    for (auto it = begin(V); it != end(V); it++) {
        if (any_of(it + 1, end(V), [&](auto& x) { return x == *it; })) {
            return false;
        }
    }
    return true;
}

// 实验框架中，采用带标记的类型以区分
struct Mint {
    int data;
    int mark;
    bool operator<=(const Mint& other) const {
        return data <= other.data;
    }
    bool operator==(const Mint& other) const {
        return data == other.data;
    }
    bool equals(const Mint& other) const {
        return data == other.data && mark == other.mark;
    }
};

Random random;
Vector<Mint> markedRandomVector(int n, int lo, int hi) {
    Vector<Mint> result(n);
    for (int i : views::iota(0, n)) {
        result.push_back({random.nextIntBetween(lo, hi), i});
    }
    return result;
}

// 为了让元素有比较多的重复，这里规定一个ratio，选取数字的区间是规模乘上这个比例
const double ratio = 0.5;

int main() {
    auto algorithms = generateInstances<Deduplicate<Mint>, DirectDeduplicate<Mint>, SortBasedDeduplicate<Mint>>();
    int testData[] { 10, 100, 1000, 10000, 100'000 };
    for (int n : testData) {
        // 生成实验数据
        cout << "Testing n = " << n << endl;
        auto V = markedRandomVector(n, 0, (int)(n * ratio));
        // 这里为了验证正确性，先不进行时间测量
        // 采取直接唯一化作为参照物
        auto basicAlgorithm = make_shared<DirectDeduplicate<Mint>>();
        auto V_b = V;
        int answer_b = basicAlgorithm->apply(V_b);
        for (auto deduplicate : algorithms) {
            auto V_t = V;
            int answer_t = deduplicate->apply(V_t);
            assert(answer_b == answer_t);
            assert(V_b.size() == V_t.size());
            assert(checkDeduplicated(V_t));
            for (int i : views::iota(0, V_t.size())) {
                assert(V_b[i].equals(V_t[i]));
            }
        }
        // 然后进行时间测量
        for (auto algorithm : algorithms) {
            auto V_t = V;
            applyTest<Deduplicate<Mint>>(algorithm, [&](auto deduplicate) {
                cout << " removed " << setw(9) << deduplicate->apply(V_t);
            });
        }
    }
    return 0;
}

