#include "list.h"
#include "vector.h"
#include "random.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子展示了在空间不连续的情况下
// 链表操作需要付出的时间是增加的

template <typename T>
using BidirectionalList = clazy::List<T>;

// 由于实验中不方便模拟真实的空间不连续环境
// 这里设计了这样的一个场景：
// 对于链表来说，我们将它的所有元素的位置打乱
// 然后按照这个次序将它们依次删除（因为单向链表有安全性问题，所以这里不对单向链表做实验）

// 使用向量来存这些元素位置
template <typename T>
using Vector = clazy::Vector<T>;

template <typename T>
class ClearProblem : public Algorithm {
public:
    virtual void initialize(int n) = 0;
    virtual void apply() = 0;
};

// 顺序删除
template <typename T>
class SequentialClear : public ClearProblem<T> {
protected:
    BidirectionalList<T> L;
    Vector<ListNodePos<T>> V;
public:
    virtual void initialize(int n) {
        L.clear();
        V.clear();
        for (int i : views::iota(0, n)) {
            V.push_back(L.insertAsPred(L.end().base(), i));
        }
    }
    virtual void apply() {
        for (auto pos : V) {
            L.remove(pos);
        }
    }
};

// 乱序删除
template <typename T>
class RandomClear : public SequentialClear<T> {
public:
    virtual void initialize(int n) {
        SequentialClear<T>::initialize(n);
        random_shuffle(begin(SequentialClear<T>::V), end(SequentialClear<T>::V));
    }
};

int main() {
    auto algorithms = generateInstances<
        ClearProblem<int>,
        SequentialClear<int>,
        RandomClear<int>
    >();
    int testData[] { 10, 1000, 10000, 100'000, 1'000'000, 10'000'000, 50'000'000 };
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        for (auto clear : algorithms) {
            clear->initialize(n);
        }
        applyTest<ClearProblem<int>>(algorithms, [&](auto clear) {
            clear->apply();
        });
    }
}

// 可以看出，乱序删除需要的时间，显著高于顺序删除
// 同样是n次删除，为什么会不一样呢？
// 这主要是os内存管理机制的影响，内存上的地址不连续会降低读写速度
// 插入、查找的情况也是相似的
// 同时，链表需要的空间也显著高于向量
// 因此，链表更多是用于需要按位置操作的场景