import Framework;
import PriorityQueue;
import Vector;
import std;

using namespace std;

using dslab::Algorithm;
using dslab::TestItem;
using dslab::Vector;

template <typename T>
using PriorityQueue = dslab::Heap<T>;

// 测试流程：
// 1. 基于N个元素创建优先队列，规模=N
// 2. 弹出N个元素，规模=0
// 3. 插入N个元素（复制），规模=N
// 4. 插入N个元素（移动），规模=N

constexpr size_t N { 5 };

// 使用std::priority_queue和我们自己实现的队列进行对拍
priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> stdPq {};

// 用于随机生成元素序列
vector<size_t> seq {};

void generateSeq() {
    static default_random_engine e { random_device{}() };
    seq.resize(2 * N);
    iota(begin(seq), end(seq), 0);
    shuffle(begin(seq), end(seq), e);
}

void check(PriorityQueue<TestItem>& PQ) {
    if (PQ.empty() != stdPq.empty()) {
        throw runtime_error("empty not match");
    }
    if (!PQ.empty()) {
        if (PQ.top().m_value != stdPq.top()) {
            throw runtime_error(format("top not match: {} vs {}", PQ.top().m_value, stdPq.top()));
        }
    }
}

class CreatePriorityQueue : public Algorithm<void(PriorityQueue<TestItem>&)> {
public:
    void operator()(PriorityQueue<TestItem>& PQ) override {
        // random N elements
        Vector<TestItem> V;
        for (size_t i { 0 }; i < N; ++i) {
            TestItem tmp { seq[i] };
            V.push_back(tmp);
            stdPq.push(seq[i]);
        }
        cout << "V  = " << V << endl;
        PQ = PriorityQueue<TestItem>(move(V));
        cout << "PQ = " << PQ << endl;
        check(PQ);
    }
    string type_name() const override {
        return "Create Priority Queue";
    }
};

class PopPriorityQueue : public Algorithm<void(PriorityQueue<TestItem>&)> {
public:
    void operator()(PriorityQueue<TestItem>& PQ) override {
        for (size_t i { 0 }; i < N; ++i) {
            auto t { PQ.pop() };
            stdPq.pop();
            cout << format("pop {} -> ", t.m_value) << PQ << endl;
            check(PQ);
        }
    }
    string type_name() const override {
        return "Pop Priority Queue";
    }
};

class PushCopyPriorityQueue : public Algorithm<void(PriorityQueue<TestItem>&)> {
public:
    void operator()(PriorityQueue<TestItem>& PQ) override {
        TestItem tmp { 0 };
        for (size_t i { 0 }; i < N; i++) {
            tmp.m_value = seq[i];
            PQ.push(tmp);
            stdPq.push(seq[i]);
            cout << format("push {} -> ", tmp.m_value) << PQ << endl;
            check(PQ);
        }
    }
    string type_name() const override {
        return "Push Copy Priority Queue";
    }
};

class PushMovePriorityQueue : public Algorithm<void(PriorityQueue<TestItem>&)> {
public:
    void operator()(PriorityQueue<TestItem>& PQ) override {
        for (size_t i { 0 }; i < N; i++) {
            PQ.push(TestItem { seq[i + N] });
            stdPq.push(seq[i + N]);
            cout << format("push {} -> ", seq[i + N]) << PQ << endl;
            check(PQ);
        }
    }
    string type_name() const override {
        return "Push Move Priority Queue";
    }
};

dslab::StructureTestFramework<
    PriorityQueue, TestItem,
    CreatePriorityQueue,
    PopPriorityQueue,
    PushCopyPriorityQueue,
    PushMovePriorityQueue
> test;

int main() {
    generateSeq();
    PriorityQueue<TestItem> PQ {};
    test.initialize();
    test(PQ);
    return 0;
}