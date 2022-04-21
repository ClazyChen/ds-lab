#include "vector.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示的是扩容策略的问题
// 一种很朴素的思想是固定扩容
// 而在clazy::DefaultVectorAllocator里的实现则是加倍扩容
// 经过摊还分析可以知道，加倍扩容的时间效率是高于固定扩容的
// 这里用实验说明这个问题

// 实现以step为步长的固定扩容策略
// 这里定义扩容策略继承于DefaultVectorAllocator
// 从而继承了永不缩容的策略
template <int step> requires (step > 0)
class APVectorAllocator : public clazy::DefaultVectorAllocator {
protected:
    virtual pair<Result, int> expand(int capacity, int size) const {
        capacity += step; // 固定扩容
        capacity = max(capacity, size);
        capacity = max(capacity, clazy::default_min_capacity);
        return {Result::Expand, capacity};
    }
};

// 实现以ratio为倍数的加倍扩容策略
template <int ratio> requires (ratio > 1)
using GPVectorAllocator = clazy::RatioAllocator<ratio, 1>;

// 当您需要增加自己设计的策略时，和上述方法类似，继承于AbstractAllocator

// 统一使用int类型的向量，对比不同的扩容策略
template <typename Alloc>
using Vector = clazy::Vector<int, Alloc>;

// 以下是实验框架代码
// 下面讨论连续在末尾插入n个元素的情况，进行实验
// 在末尾插入元素可以降低插入元素本身花费的时间，从而更明显地观察扩容的时间
class InsertProblem : public Algorithm<void, int> {
};

template <typename Alloc>
class InsertTest : public InsertProblem {
protected:
    Vector<Alloc> V;
public:
    void apply(int n) override {
        for (int i = 0; i < n; i++) {
            V.push_back(i);
        }
        assert(V.size() == n);
    }

    // 重载清空函数，以便重置辅助数据结构
    void clear() override {
        V.reserve(0);
    }

    string getTypeName() const override {
        return Alloc().getTypeName();
    }
};

// 实验中使用的数据规模
int testData[] { 100, 1000, 10000, 100'000, 1'000'000 };

// 进行实验时，将您设计的Allocator添加进来即可
int main() {
    TestFramework<
        InsertProblem,
        InsertTest<APVectorAllocator<64>>,
        InsertTest<APVectorAllocator<4096>>,
        InsertTest<GPVectorAllocator<2>>,
        InsertTest<GPVectorAllocator<4>>
    > tf;
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        tf.clear();
        tf.test(n);
    }
    return 0;
}
