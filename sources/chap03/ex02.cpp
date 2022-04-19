#include "list.h"
#include "vector.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示了在空间不连续的情况下
// 链表操作需要付出的时间是增加的

template <typename T>
using BidirectionalList = clazy::List<T>;

// 由于实验中不方便模拟真实的空间不连续环境
// 这里设计了这样的一个场景：
// 对于链表来说，我们将它的所有元素的位置打乱
// 然后按照这个混乱的次序将它们依次删除
// 因为单向链表有安全性问题，所以这里不对单向链表做实验

// 使用向量来存这些元素位置
template <typename T>
using Vector = clazy::Vector<T>;

class ClearProblem : public Algorithm<> {
protected:
    BidirectionalList<int> L;   // 用来存放元素的链表
    Vector<ListNodePos<int>> V; // 删除的次序
public:
    virtual void initialize(int n) = 0; // 初始化删除的次序
    void apply() override {   // 按照次序删除
        for (auto&& pos : V) {
            L.remove(pos);
        }
    }
    void clear() override {
        L.clear();
        V.clear();
    }
};

// 顺序删除
class SequentialClear : public ClearProblem {
public:
    void initialize(int n) override {
        for (int i = 0; i < n; i++) {
            V.push_back(L.insertAsSucc(L.getBackPos(), i));
        }
    }
};

// 乱序删除
class RandomClear : public SequentialClear {
public:
    void initialize(int n) override {
        SequentialClear::initialize(n);
        random_shuffle(begin(this->V), end(this->V));
    }
};

int testData[] { 10, 1000, 10000, 100'000, 1'000'000, 10'000'000, 50'000'000 };

int main() {
    TestFramework<ClearProblem, SequentialClear, RandomClear> tf;
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        for (auto&& instance : tf.instances) {
            instance->initialize(n);
        }
        tf.test();
        tf.clear();
    }
}

// 可以看出，乱序删除需要的时间，显著高于顺序删除
// 同样是n次删除，为什么会不一样呢？
// 这主要是os内存管理机制的影响，内存上的地址不连续会降低读写速度
// 插入、查找的情况也是相似的