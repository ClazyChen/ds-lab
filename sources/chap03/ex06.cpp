#include "framework.h"
using namespace clazy_framework;

// 这个例子说明，为什么静态链表会被引入
// 静态链表事实上对应的是不需要进行内存管理的情况
// 也就是，真正的“静态”的情况
// 比如，在链表的整个生命周期中产生的节点数有一个固定的上界
// 或者，链表被生成之后，就不会再发生变化（如图中的邻接表）
// 这些情况下，静态链表可以简单地用数组代替向量，并且可以节约掉用来内存回收的栈
// 这个时候，静态链表事实上不需要处理destroy
// 并且在create的时候，也可以简单地取到数组中的下一个未被使用的元素，这个速度是比依次new更快的

// 特别提醒：在进行这个实验的时候，您需要将-O2关闭
// 因为这里申请内存之后什么事情都没有做，所有如果打开-O2会被优化掉

class Malloc : public Algorithm {
protected:
    unique_ptr<char[]> _data;
public:
    virtual void apply(int n) = 0;
    virtual void reset() { _data = nullptr; }
};

class BatchedMalloc : public Malloc {
public:
    virtual void apply(int n) {
        this->_data = make_unique<char[]>(n);
    }
};

class IndividualMalloc : public Malloc {
public:
    virtual void apply(int n) {
        for (int i : views::iota(0, n)) {
            auto _ = new char; // 为了展示new only的时间，这里不能做别的事情，也就没法delete了
        }
    }
};

int main() {
    auto algorithms = generateInstances<Malloc, BatchedMalloc, IndividualMalloc>();
    int testData[] { 100'000, 1'000'000, 10'000'000 };
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        for (auto algorithm : algorithms) {
            algorithm->reset();
        }
        applyTest<Malloc>(algorithms, [n](auto algorithm) {
            algorithm->apply(n);
        });
    }
}