#include "vector.h"
#include <cassert>
using namespace clazy_framework;
using clazy::DefaultVectorAllocator;

// 这个例子展示的是扩容策略的问题
// 一种很朴素的思想是固定扩容
// 而在clazy::DefaultVectorAllocator里的实现则是加倍扩容
// 经过摊还分析可以知道，加倍扩容的时间效率是高于固定扩容的
// 这里用实验说明这个问题

// 这里定义扩容策略继承于DefaultVectorAllocator
// 从而继承了永不缩容的策略

// 实现以step为步长的固定扩容策略
template <int step> requires (step > 0)
class APVectorAllocator : public DefaultVectorAllocator {
protected:
    virtual pair<Result, int> expand(int capacity, int size) const {
        while (capacity < size) {     // 固定扩容策略
            capacity += step;
        }
        return {Result::Expand, capacity};
    }
};

// 实现以ratio为倍数的加倍扩容策略
template <int ratio> requires (ratio > 1)
class GPVectorAllocator : public DefaultVectorAllocator {
protected:
    virtual pair<Result, int> expand(int capacity, int size) const {
        while (capacity < size) {     // 加倍扩容策略
            capacity *= ratio;
        }
        return {Result::Expand, capacity};
    }
};

// 当您需要增加自己设计的策略时，和上述方法类似，继承于VectorAllocator

template <typename Allocator> requires (is_base_of_v<VectorAllocator, Allocator>)
using Vector = clazy::Vector<int, Allocator>;

// 当您需要使用自己实现的向量时，修改上述Vector的指向目标

// 以下是实验框架代码
// 下面讨论连续在末尾插入n个元素的情况，进行实验
// 在末尾插入元素可以降低插入元素本身花费的时间，从而更明显地观察扩容的时间
template <typename Allocator> requires (is_base_of_v<VectorAllocator, Allocator>)
class InsertTest : public Algorithm {
protected:
    Vector<Allocator> V;
public:
    virtual string getTypename() const {
        return typeid(Allocator).name(); // 使用Allocator的名字以降低长度
    }
    void apply(int n) {
        for (int i = 0; i < n; i++) {
            V.push_back(i);
        }
        assert(V.size() == n);
    }
};

// 对一种参数策略和一种n进行实验，得到时间
template <typename Allocator> requires (is_base_of_v<VectorAllocator, Allocator>)
void applyTest(int n) {
    auto instance = make_shared<InsertTest<Allocator>>();
    applyTest<InsertTest<Allocator>>(instance, [&](auto test) {
        test->apply(n);
    });
}

// 实验中使用的数据规模
int testData[] { 100, 1000, 10000, 100'000, 1'000'000 };

// 进行实验的函数
template <typename... Allocator> requires (is_base_of_v<VectorAllocator, Allocator> && ...)
void applyAllTests() {
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        int _[] = {(applyTest<Allocator>(n), 0)... }; // 这种写法为了有序地展开各个Allocator
    }
}

// 进行实验时，将您设计的Allocator添加进来即可
int main() {
    applyAllTests<
        APVectorAllocator<64>,
        APVectorAllocator<4096>,
        GPVectorAllocator<2>,
        GPVectorAllocator<3>
        >();
    return 0;
}
