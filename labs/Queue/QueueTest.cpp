#include <vector>
#include <format>
#include <iostream>
import Framework;
import Queue;
import Queue.StackQueue;
using namespace std;

using dslab::Algorithm;
using dslab::TestItem;

// 这个实验用于展示和验证队列的基本操作
// 您可以将下面的队列替换为自己实现的队列参与测试
// 顺序队列（整体搬移、循环或双栈）、链式队列都可以
template <typename T>
using Queue = dslab::Queue<T>;

// 测试流程：
// 1. 创建一个空队列，规模=0
// 2. 在队尾插入N个元素（复制），规模=N
// 3. 在队尾插入N个元素（移动），要求不触发复制，规模=2N
// 4. 在队头删除2N个元素，要求不触发复制，规模=0

constexpr size_t N { 5 };

// 使用std::vector和我们自己实现的队列进行对拍
vector<size_t> stdVector {};

void check(Queue<TestItem>& Q) {
    if (Q.empty() != stdVector.empty()) {
        throw runtime_error("empty not match");
    }
    if (!Q.empty()) {
        if (Q.front().m_value != stdVector.front()) {
            throw runtime_error("front not match");
        }
    }
}

class CreateQueue : public Algorithm<void(Queue<TestItem>&)> {
public:
    void operator()(Queue<TestItem>& Q) override {
        Q = {};
        stdVector = {};
        check(Q);
    }
};

class PushCopyQueue : public Algorithm<void(Queue<TestItem>&)> {
public:
    void operator()(Queue<TestItem>& Q) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; i++) {
            tmp.m_value = i;
            Q.enqueue(tmp);
            stdVector.push_back(i);
            cout << format("push {} -> ", i) << Q << endl;
            check(Q);
        }
        if (Q.size() != N) {
            throw runtime_error("size != N");
        }
    }
};

class PushMoveQueue : public Algorithm<void(Queue<TestItem>&)> {
public:
    void operator()(Queue<TestItem>& Q) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < N; i++) {
            Q.enqueue(TestItem { i + N });
            stdVector.push_back(i + N);
            cout << format("push {} -> ", i + N) << Q << endl;
            check(Q);
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (Q.size() != 2 * N) {
            throw runtime_error("size != 2 * N");
        }
    }
};

class PopQueue : public Algorithm<void(Queue<TestItem>&)> {
public:
    void operator()(Queue<TestItem>& Q) override {
        TestItem::s_copyCount = 0;
        TestItem::s_moveCount = 0;
        for (size_t i { 0 }; i < 2 * N; i++) {
            Q.dequeue();
            stdVector.erase(stdVector.begin());
            cout << format("pop -> ") << Q << endl;
            check(Q);
        }
        if (TestItem::s_copyCount != 0) {
            throw runtime_error("copyCount != 0");
        }
        if (Q.size() != 0) {
            throw runtime_error("size != 0");
        }
    }
};

dslab::StructureTestFramework<
    Queue, TestItem,
    CreateQueue, PushCopyQueue, PushMoveQueue, PopQueue> test;

int main() {
    Queue<TestItem> Q {};
    test.initialize();
    test(Q);
    return 0;
}