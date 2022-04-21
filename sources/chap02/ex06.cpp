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

// 实验包括两个部分
// 第一个部分是测试排序并进行验证
// 在生成随机数的时候，我们使用RandomVector去生成0-65535之间的随机数
// 并通过除余法将其限制在一定的范围里
// 在这里，范围取做0-2n之间的随机数，这样有些数可能被选中了多次，而另一些数则不会被选中
// 如果您修改了这个系数2，比如，将它降低到1甚至更低，那么您将会看到更多的数被选中
// 这就意味着在查找的过程中更有可能以成功为返回；反之如果这个系数更大，则更有可能以失败为返回
const int random_ratio = 2.0;
int rangeLimit(int n) {
    return (int)(n * random_ratio);
}

// 第二个部分是测试查找的性能
// 为了充分体现查找本身的性能，这里需要提前生成查找的数据
// 查找的数据量是固定的，不随n的变化而变化
// 接受的参数表示待查找的向量、待查找的数据组成的向量
const int search_count = 100'000;

class ExperimentalSearchProblem : public Algorithm<void, const Vector<int>&, const Vector<int>&> {

};

template <typename S>
requires (is_base_of_v<AbstractSearch<int, Rank, Vector<int>>, S>)
class ExperimentalSearch : public ExperimentalSearchProblem {
private:
    S search_algorithm;
public:
    void apply(const Vector<int>& V, const Vector<int>& search_data) override {
        for (int e : search_data) {
            search_algorithm(V, e);
        }
    }

    void clear() override {
        search_algorithm.clear();
    }

    string getTypeName() const override {
        return search_algorithm.getTypeName();
    }
};

// 下面描述实验框架，它接收一个数字n，表示要测试的数据量
class Experiment : public Algorithm<void, int> {
private:
    // 用来做测试的向量
    Vector<int> V;

    // 用来做查找的数据
    Vector<int> search_data;

    // 向量的排序算法
    TestFramework<AbstractSort<int, Vector<int>>,
        clazy::VectorMergeSort<int, Vector<int>>> tf_sort;

    // 向量的查找算法
    TestFramework<ExperimentalSearchProblem,
        ExperimentalSearch<clazy::SequentialSearch<int, Rank, Vector<int>>>,
        ExperimentalSearch<clazy::BinarySearch<int, Vector<int>>>> tf_search;

public:
    // 初始化：生成一个随机的乱序向量
    void initialize(Vector<int>& V, int n) {
        V = clazy::RandomVector<int, Vector<int>>()(n, 0, rangeLimit(n));
    }

    // 第一个实验：排序
    void experiment1() {
        tf_sort.clear();
        tf_sort.apply(V);
        assert(is_sorted(begin(V), end(V)));
    }

    // 第二个实验：查找，比较各个查找算法的性能
    void experiment2() {
        tf_search.clear();
        tf_search.test(V, search_data);
    }

    // 实验流程
    void apply(int n) override {
        initialize(V, n);
        initialize(search_data, search_count);
        experiment1();
        experiment2();
    }

    void clear() override {
        V.clear();
    }

};

int testData[] { 10, 100, 1000, 10000 };

int main() {
    Experiment experiment;
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        experiment(n);
    }
    return 0;
}
