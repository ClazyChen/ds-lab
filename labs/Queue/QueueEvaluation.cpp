import Queue;
import Queue.StackQueue;
import Framework;
import Vector;
import Stack.RandomStackOperation;
import std;

using namespace dslab;
using namespace std;

// 对双栈队列进行优化，不采用共享栈，以降低判断和移动的次数
template <typename T, template<typename> typename Vec = DefaultVector>
    requires std::is_base_of_v<AbstractVector<T>, Vec<T>>
class DualStackQueue : public AbstractQueue<T> {
    Vec<T> backStack;
    Vec<T> frontStack;
public:
    void enqueue(const T& e) override {
        backStack.push_back(e);
    }
    void enqueue(T&& e) override {
        backStack.push_back(e);
    }
    T dequeue() override {
        if (frontStack.empty()) {
            frontStack.resize(backStack.size());
            move(rbegin(backStack), rend(backStack), begin(frontStack));
            backStack.clear();
        }
        return frontStack.pop_back();
    }
    T& front() override {
        if (frontStack.empty()) {
            frontStack.resize(backStack.size());
            move(rbegin(backStack), rend(backStack), begin(frontStack));
            backStack.clear();
        }
        return frontStack.back();
    }
    size_t size() const override {
        return backStack.size() + frontStack.size();
    }
    string type_name() const override {
        return std::format("Queue with dual Stack ({})", frontStack.type_name());
    }
};

// C++ STL中的队列，用于对比
// std::queue默认是基于std::deque实现的，比较慢
template <typename T>
class QueueStd : public AbstractQueue<T> {
    std::queue<T> Q;
public:
void enqueue(const T& e) override {
        Q.push(e);
    }
    void enqueue(T&& e) override {
        Q.push(e);
    }
    T dequeue() override {
        T e = Q.front();
        Q.pop();
        return e;
    }
    T& front() override {
        return Q.front();
    }
    size_t size() const override {
        return Q.size();
    }
    string type_name() const override {
        return "std::queue";
    }
};

template <typename T>
class QueueTestProblem : public Algorithm<void(size_t)> {
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
    string type_name() const override {
        return this->Q.type_name();
    }
};

RandomStackOperation<char, string> randop;
string opstr;

template <typename T, typename Que>
    requires is_base_of_v<AbstractQueue<T>, Que>
class EnqueueDequeueTest : public QueueTest<T, Que> {
public:
    void operator()(size_t n) override {
        for (char c : opstr) {
            if (c == '(') {
                this->Q.enqueue(1);
            } else {
                this->Q.dequeue();
            }
        }
    }
    string type_name() const override {
        return this->Q.type_name();
    }
};

vector testData { 10, 1000, 10000, 100'000, 1'000'000, 10'000'000 };

TestFramework<QueueTestProblem<int>,
    EnqueueTest<int, Queue<int, DefaultVector, ratio<1,2>>>,
    EnqueueTest<int, Queue<int>>,
    EnqueueTest<int, Queue<int, DefaultVector, ratio<1>>>,
    EnqueueTest<int, LinkedQueue<int>>,
    EnqueueTest<int, CircularQueue<int>>,
    EnqueueTest<int, StackQueue<int>>,
    EnqueueTest<int, DualStackQueue<int>>,
    EnqueueTest<int, QueueStd<int>>
> test_enq;

TestFramework <QueueTestProblem<int>,
    EnqueueDequeueTest<int, Queue<int, DefaultVector, ratio<1, 2>>>,
    EnqueueDequeueTest<int, Queue<int>>,
    EnqueueDequeueTest<int, Queue<int, DefaultVector, ratio<1>>>,
    EnqueueDequeueTest<int, LinkedQueue<int>>,
    EnqueueDequeueTest<int, CircularQueue<int>>,
    EnqueueDequeueTest<int, StackQueue<int>>,
    EnqueueDequeueTest<int, DualStackQueue<int>>,
    EnqueueDequeueTest<int, QueueStd<int>>
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
        opstr = randop(n, '(', ')');
        test_enq_deq.run(&QueueTestProblem<int>::initialize);
        test_enq_deq(n);
    }
}