#include "queue_vector.h"
#include "queue_circular.h"
using namespace clazy_framework;

// 这个例子比较三种队列的性能

template <typename T>
using LinkedQueue = clazy::Queue<T>;

template <typename T>
using SequentialQueue = clazy::VectorQueue<T>;

template <typename T>
using CircularQueue = clazy::CircularQueue<T>;

// 比较两种情况：连续插入和连续删除
// 这不能体现出循环队列的搬移，因为操作序列太过简单
// 循环队列的搬移操作的时间量级可以参考向量扩容
template <typename T>
class EnqueueDequeueProblem : public Algorithm {
public:
    virtual void enqueue(int n, const T& dummy);
    virtual void dequeue();
};

template <typename T, typename Queue>
requires (is_base_of_v<clazy_framework::AbstractQueue<T>, Queue>)
class EnqueueDequeue : public EnqueueDequeueProblem<T> {
protected:
    Queue Q;
public:
    virtual void enqueue(int n, const T& dummy) override {
        for (int i : views::iota(0, n)) {
            Q.enqueue(dummy);
        }
    }
    virtual void dequeue() override {
        while (!Q.empty()) {
            Q.dequeue();
        }
    }
    virtual string getTypename() const override {
        return typeid(Queue).name();
    }
};

int main() {
    auto algorithms = generateInstances<
        EnqueueDequeueProblem<int>,
        EnqueueDequeue<int, LinkedQueue<int>>,
        EnqueueDequeue<int, SequentialQueue<int>>,
        EnqueueDequeue<int, CircularQueue<int>>>();
    int testData[] { 10, 1000, 100'000, 1'000'000, 10'000'000, 20'000'000 };
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        cout << "Testing Enqueue" << endl;
        applyTest<EnqueueDequeueProblem<int>>(algorithms, [n] (auto algorithm) {
            algorithm->enqueue(n, 0);
        });
        cout << "Testing Dequeue" << endl;
        applyTest<EnqueueDequeueProblem<int>>(algorithms, [n] (auto algorithm) {
            algorithm->dequeue();
        });
    }
}

// 向量实现的队列（基于搬移），效率显著地高于其他两种
// 向量和列表的性能差异毋庸赘述，而在循环队列中，每次操作都需要取模运算，因而对性能造成了一定影响
// 可以预见，循环队列考虑最坏情况的搬移，性能仍然明显高于链式队列