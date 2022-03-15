#include "vector_sort.h"
#include "vector_search.h"
#include "random.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子展示向量的排序
// 以及折半查找相对顺序查找的优越性
// 需要说明的是，在这里并不讨论排序和查找本身
// 我们将这个内容放到排序、查找的单独章里进行实验

template <typename T>
using Vector = clazy::Vector<T>;

// 随机数发生器
Random random;

const int random_ratio = 2;       // 从一定倍数的区间里随机选取，这样区间里有些数选不到
const int search_count = 100'000; // 查找操作的次数
constexpr int rangeLimit(int n) {
    return n * random_ratio;
}
constexpr int testData[] {
    10, 100, 1000, 10000, 100'000
};

int main() {
    shared_ptr<Sort<int, Vector<int>>> sortAlgorithm = make_shared<clazy::VectorMergeSort<int>>();
    auto searchAlgorithms = generateInstances<clazy::VectorSearch<int, Vector<int>>, clazy::VectorSequentialSearch<int>, clazy::VectorBinarySearch<int>>();
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        auto V = randomVector<Vector<int>>(n, 0, rangeLimit(n));
        applyTest<Sort<int, Vector<int>>>(sortAlgorithm, [&](auto vectorSort) {
            vectorSort->apply(V);
        });
        assert(is_sorted(begin(V), end(V)));
        // 这里预先生成好要查找的元素，从而在比较时不需要考虑随机数的时间开销
        auto VF = randomVector<Vector<int>>(search_count, 0, rangeLimit(n));
        // 算法正确性验证，采用顺序查找作为参照物
        auto basicSearch = make_shared<clazy::VectorSequentialSearch<int>>();
        for (int i : views::iota(0, (int)sqrt(VF.size()))) {
            auto [res_b, r_b] = basicSearch->apply(V, VF[i]);
            for (auto search : searchAlgorithms) {
                auto [res, r] = search->apply(V, VF[i]);
                assert(res == res_b && r == r_b);
            }
        }
        // 算法性能对比
        applyTest<clazy::VectorSearch<int, Vector<int>>>(searchAlgorithms, [&](auto vectorSearch) {
            for (int x : VF) {
                vectorSearch->apply(V, x);
            }
        });
    }
    return 0;
}
