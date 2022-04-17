#include "vector.h"
#include "random.h"
#include "vector_sort.h"
#include "vector_search.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示向量的排序
// 以及折半查找相对顺序查找的优越性
// 需要说明的是，在这里并不讨论排序和查找本身
// 我们将这个内容放到排序、查找的单独章里进行实验

template <typename T>
using Vector = clazy::Vector<T>;

const double random_ratio = 1.5;       // 从一定倍数的区间里随机选取，这样区间里有些数选不到
const int search_count = 100'000;      // 查找操作的次数
constexpr int rangeLimit(int n) {
    return (int)(n * random_ratio);
}


// 下面描述实验框架，它接收一个数字n，表示要测试的数据量
class Experiment : public Algorithm<void, int> {
private:
    // 用来做测试的向量
    Vector<int> V;

    // 向量的排序算法
    TestFramework<AbstractSort<int, Vector<int>>,
        clazy::VectorMergeSort<int, Vector<int>>> tf_sort;

    // 向量的查找算法
    TestFramework<AbstractSearch<int, Rank, Vector<int>>,
        clazy::SequentialSearch<int, Rank, Vector<int>>,
        clazy::BinarySearch<int, Vector<int>>> tf_search;

public:
    // 初始化：生成一个随机的乱序向量
    void initialize(int n) {
        auto v = clazy::RandomVector()(n);
        V.resize(v.size());
        for (int i = 0; i < v.size(); i++) {
            V[i] = v[i] % rangeLimit(n);
        }
    }

    // 第一个实验：排序，并验证排序结果是否正确
    void sortExperiment() {
        tf_sort.test(V);
        assert(is_sorted(begin(V), end(V)));
    }

    // 第二个实验：查找，比较各个查找算法的性能
    

    // 
    void apply(int n) override {

    }

    void clear() override {
        V.clear();
    }

};

int testData[] { 10, 100, 1000, 10000, 100'000 };

int main() {
    shared_ptr<Sort<int, Vector<int>>> sortAlgorithm = make_shared<clazy::VectorMergeSort<int>>();
    auto searchAlgorithms = generateInstances<
        clazy::VectorSearch<int, Vector<int>>,
        clazy::VectorSequentialSearch<int>,
        clazy::VectorBinarySearch<int>>();
    
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
        auto basicSearch = searchAlgorithms[0];
        for (int i : views::iota(0, (int)sqrt(VF.size()))) {
            auto [res_b, r_b] = basicSearch->apply(V, VF[i]);
            for (auto search : searchAlgorithms | views::drop(1)) {
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
