#include <vector>
#include <format>
#include <iostream>
import Queue;
import Framework;
using namespace dslab;
using namespace std;

template <typename T>
class QueueTestProblem : public Algorithm<void, size_t> {
public:
    virtual void initialize() = 0;
};

template <typename T, typename Que>
    requires is_base_of_v<AbstractQueue<T>, Que>
class QueueTest : public QueueTestProblem<T> {
protected:
    Que Q;
public:
    void initialize() override {
        while (!Q.empty()) {
            Q.dequeue();
        }
    }

    string type_name() const override {
        return Q.type_name();
    }
};

template <typename T, typename Que>
    requires is_base_of_v<AbstractQueue<T>, Que>
class EnqueueTest : public QueueTest<T, Que> {
public:
    void operator()(size_t n) override {
        for (size_t i { 0 }; i < n; ++i) {
            this->Q.enqueue(i);
        }
    }
};

template <typename T, typename Que>
    requires is_base_of_v<AbstractQueue<T>, Que>
class EnqueueDequeueTest : public QueueTest<T, Que> {
public:
    void operator()(size_t n) override {
        for (size_t i { 0 }; i < n / 2; ++i) {
            this->Q.enqueue(i);
        }
        for (size_t i { 0 }; i < n / 4 + 1; ++i) {
            this->Q.dequeue();
        }
        for (size_t i { n / 2 }; i < n; ++i) {
            this->Q.enqueue(i);
        }
        for (size_t i { n / 4 + 1 }; i < n; ++i) {
            this->Q.dequeue();
        }
    }
};

vector testData { 10, 1000, 10000, 100'000, 1'000'000, 10'000'000 };

TestFramework<QueueTestProblem<int>,
    EnqueueTest<int, Queue<int>>,
    EnqueueTest<int, Queue<int, DefaultVector, ratio<1>>>,
    EnqueueTest<int, LinkedQueue<int>>,
    EnqueueTest<int, CircularQueue<int>>
> test_enq;

TestFramework <QueueTestProblem<int>,
    EnqueueDequeueTest<int, Queue<int>>,
    EnqueueDequeueTest<int, Queue<int, DefaultVector, ratio<1>>>,
    EnqueueDequeueTest<int, LinkedQueue<int>>,
    EnqueueDequeueTest<int, CircularQueue<int>>
> test_enq_deq;

int main() {
    cout << "Enqueue n times test:" << endl;
    for (auto n : testData) {
        cout << format("n = {}", n) << endl;
        test_enq.run(&QueueTestProblem<int>::initialize);
        test_enq(n);
    }
    cout << "Enqueue n & Dequeue n times test:" << endl;
    for (auto n : testData) {
        cout << format("n = {}", n) << endl;
        test_enq_deq.run(&QueueTestProblem<int>::initialize);
        test_enq_deq(n);
    }
}