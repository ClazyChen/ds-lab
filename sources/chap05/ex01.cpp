#include "queue.h"
#include "queue_linked.h"
#include "queue_circular.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子比较三种队列的性能

template <typename T>
using LinkedQueue = clazy::LinkedQueue<T>;

template <typename T>
using SequentialQueue = clazy::Queue<T>;

template <typename T>
using CircularQueue = clazy::CircularQueue<T>;

// 这里设计的实验场景是n次enqueue+n次dequeue
// 1. enqueue n/2 个元素
// 2. dequeue n/4+1 个元素（触发一次搬移版本的搬移）
// 3. enqueue n/2 个元素（触发一次循环版本的扩容时移动）
// 4. dequeue 3n/4-1 个元素（触发若干次搬移版本的搬移）

template <typename T>
class EnqueueDequeueProblem : public Algorithm<void, int> {
};

template <typename T, typename Queue>
requires (is_base_of_v<clazy_framework::AbstractQueue<T>, Queue>)
class EnqueueDequeue : public EnqueueDequeueProblem<T> {
protected:
    Queue Q;
public:
    void apply(int n) override {
        for (int i = 0; i < n/2; i++) {
            Q.enqueue(i);
        }
        for (int i = 0; i < n/4+1; i++) {
            Q.dequeue();
        }
        for (int i = 0; i < n/2; i++) {
            Q.enqueue(i);
        }
        for (int i = n/4+1; i < n; i++) {
            Q.dequeue();
        }
    }

    void clear() override {
        Q.clear();
    }

    string getTypeName() const override {
        return typeid(Queue).name();
    }
};

int testData[] { 10, 1000, 100'000, 1'000'000, 10'000'000, 20'000'000 };

int main() {
    TestFramework<
        EnqueueDequeueProblem<int>,
        EnqueueDequeue<int, LinkedQueue<int>>,
        EnqueueDequeue<int, SequentialQueue<int>>,
        EnqueueDequeue<int, CircularQueue<int>>> tf;
    for (int n : testData) {
        cout << "Testing n = " << n << endl;
        tf.test(n);
    }
}

// 向量实现的队列效率显著高于列表实现的队列
// 两种向量实现的队列各有优劣，基于搬移的做法相对于基于循环的做法，不需要进行取模运算，性能会更高一些
// 其缺点在于向量扩容虽然分摊复杂度是O(1)的，但是不分摊的情况下最坏复杂度是O(n)的，造成了性能的不稳定
