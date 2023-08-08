import PriorityQueue.AbstractPriorityQueue;
import Vector;
import Framework;
import std;

using namespace dslab;
using namespace std;

template <typename T>
class NaiveVectorPriorityQueue : public AbstractPriorityQueue<T> {
    Vector<T> V;
    Rank min() {
        return min_element(begin(V), end(V), this->cmp) - begin(V);
    }

public:
    NaiveVectorPriorityQueue() = default;
    NaiveVectorPriorityQueue(Vector<T>&& V, const std::function<bool(const T&, const T&)>& cmp = std::less<T>()) : AbstractPriorityQueue<T>(cmp), V(move(V)) {}

    size_t size() const override {
        return V.size();
    }

    void clear() override {
        V.clear();
    }

    void push(const T& e) override {
        V.push_back(e);
    }

    void push(T&& e) override {
        V.push_back(move(e));
    }

    T pop() override {
        return V.remove(min());
    }

    const T& top() const override {
        return V[min()];
    }

    string type_name() const override {
        return "Unsorted / Naive";
    }
};

template <typename T>
class MemoryVectorPriorityQueue : public AbstractPriorityQueue<T> {
    Vector<T> V;
    optional<Rank> m;
    Rank min() {
        if (!m.has_value()) {
            m = min_element(begin(V), end(V), this->cmp) - begin(V);
        }
        return m.value();
    }
public:
    MemoryVectorPriorityQueue() = default;
    MemoryVectorPriorityQueue(Vector<T>&& V, const std::function<bool(const T&, const T&)>& cmp = std::less<T>()) : AbstractPriorityQueue<T>(cmp), V(move(V)) {}

    size_t size() const override {
        return V.size();
    }

    void clear() override {
        V.clear();
        m.reset();
    }

    void push(const T& e) override {
        V.push_back(e);
        m.reset();
    }

    void push(T&& e) override {
        V.push_back(move(e));
        m.reset();
    }

    T pop() override {
        T e { V.remove(min()) };
        m.reset();
        return e;
    }

    const T& top() const override {
        return V[min()];
    }

    string type_name() const override {
        return "Unsorted / Record Min";
    }
};

template <typename T>
class UpdMemVectorPriorityQueue : public AbstractPriorityQueue<T> {
    Vector<T> V;
    optional<Rank> m;
    Rank min() {
        if (!m.has_value()) {
            m = min_element(begin(V), end(V), this->cmp) - begin(V);
        }
        return m.value();
    }
    void update() {
        if (m.has_value() && this->cmp(V.back(), top())) {
            m = V.size() - 1;
        }
    }
public:
    UpdMemVectorPriorityQueue() = default;
    UpdMemVectorPriorityQueue(Vector<T>&& V, const std::function<bool(const T&, const T&)>& cmp = std::less<T>()) : AbstractPriorityQueue<T>(cmp), V(move(V)) {}

    size_t size() const override {
        return V.size();
    }

    void clear() override {
        V.clear();
        m.reset();
    }

    void push(const T& e) override {
        V.push_back(e);
        update();
    }

    void push(T&& e) override {
        V.push_back(move(e));
        update();
    }

    T pop() override {
        T e { V.remove(min()) };
        m.reset();
        return e;
    }

    const T& top() const override {
        return V[min()];
    }

    string type_name() const override {
        return "Unsorted / Record Min (Incremental Update)";
    }
};

// sort by decreasing order
template <typename T>
class SortedVectorPriorityQueue : public AbstractPriorityQueue<T> {
    Vector<T> V;
    function<bool(const T&, const T&)> icmp;
public:
    SortedVectorPriorityQueue() {
        icmp = [this](const T& a, const T& b) {
            return this->cmp(b, a);
        };
    }
    SortedVectorPriorityQueue(Vector<T>&& V, const std::function<bool(const T&, const T&)>& cmp = std::less<T>()) : AbstractPriorityQueue<T>(cmp), V(move(V)) {
        icmp = [this](const T& a, const T& b) {
            return this->cmp(b, a);
        };
        sort(begin(V), end(V), this->icmp);
    }

    size_t size() const override {
        return V.size();
    }
    
    void clear() override {
        V.clear();
    }

    void push(const T& e) override {
        auto pos { upper_bound(begin(V), end(V), e, this->icmp) - begin(V) };
        V.insert(pos, e);
    }

    void push(T&& e) override {
        auto pos { upper_bound(begin(V), end(V), e, this->icmp) - begin(V) };
        V.insert(pos, move(e));
    }

    T pop() override {
        return V.pop_back();
    }

    const T& top() const override {
        return V.back();
    }

    string type_name() const override {
        return "Sorted";
    }
};

template <typename T>
class PriorityQueueTest : public Algorithm<int()> {
protected:
    Vector<int> V;
public:
    virtual void initialize(const Vector<T>& V) {
        this->V = V;
    }
};

template <typename T, template <typename> typename PQ>
    requires is_base_of_v<AbstractPriorityQueue<T>, PQ<T>>
class PriorityQueueTestImpl : public PriorityQueueTest<T> {
protected:
    PQ<T> pq;
public:
    void initialize(const Vector<T>& V) override {
        PriorityQueueTest<T>::initialize(V);
        pq.clear();
    }
    string type_name() const override {
        return pq.type_name();
    }
};

template <typename T, template <typename> typename PQ>
class PriorityQueueTestImpl1 : public PriorityQueueTestImpl<T, PQ> {
public:
    int operator()() override {
        int ans { 0 };
        for (int i : this->V) {
            this->pq.push(i);
            ans += i;
        }
        return ans;
    }
};

template <typename T, template <typename> typename PQ>
class PriorityQueueTestImpl2 : public PriorityQueueTestImpl<T, PQ> {
public:
    int operator()() override {
        int ans { 0 };
        for (int i : this->V) {
            this->pq.push(i);
            ans += i;
        }
        for (int i : this->V) {
            ans += this->pq.top();
        }
        return ans;
    }
};


template <typename T, template <typename> typename PQ>
class PriorityQueueTestImpl3 : public PriorityQueueTestImpl<T, PQ> {
public:
    int operator()() override {
        int ans { 0 };
        for (int i : this->V) {
            this->pq.push(i);
            ans += this->pq.top();
        }
        return ans;
    }
};

template <typename T, template <typename> typename PQ>
class PriorityQueueTestImpl4 : public PriorityQueueTestImpl<T, PQ> {
public:
    int operator()() override {
        int ans { 0 };
        for (int i : this->V) {
            this->pq.push(i);
        }
        for (int i : this->V) {
            ans += this->pq.pop();
        }
        return ans;
    }
};

template <typename T, template <typename> typename PQ>
class PriorityQueueTestImpl5 : public PriorityQueueTestImpl<T, PQ> {
public:
    void initialize(const Vector<T>& V) override {
        auto V1 { V };
        sort(begin(V1), end(V1));
        PriorityQueueTest<T>::initialize(V1);
        this->pq.clear();
    }
    int operator()() override {
        int ans { 0 };
        for (int i : this->V) {
            this->pq.push(i);
        }
        for (int i : this->V) {
            ans += this->pq.pop();
        }
        return ans;
    }
};


template <template <typename, template<typename> typename> typename Impl>
requires is_base_of_v<PriorityQueueTest<int>, Impl<int, NaiveVectorPriorityQueue>>
class PriorityQueueExperimentImpl {
    static inline vector testData { 10, 100, 1000, 3000, 10000 };
    default_random_engine engine { random_device{}() };
    TestFramework<PriorityQueueTest<int>,
        Impl<int, NaiveVectorPriorityQueue>,
        Impl<int, MemoryVectorPriorityQueue>,
        Impl<int, UpdMemVectorPriorityQueue>,
        Impl<int, SortedVectorPriorityQueue>
    > test;
public:
    void operator()() {
        for (auto n : testData) {
            cout << format("n = {:>8}", n) << endl;
            Vector<int> V(n);
            iota(begin(V), end(V), 0);
            shuffle(begin(V), end(V), engine);
            test.run([&V](auto& algo) { algo.initialize(V); });
            test();
        }
    }
};

int main() {
    cout << "Experiment 1 : Insert only" << endl;
    PriorityQueueExperimentImpl<PriorityQueueTestImpl1>()();
    cout << endl << "Experiment 2 : Insert and then Top" << endl;
    PriorityQueueExperimentImpl<PriorityQueueTestImpl2>()();
    cout << endl << "Experiment 3 : Insert with Top" << endl;
    PriorityQueueExperimentImpl<PriorityQueueTestImpl3>()();
    cout << endl << "Experiment 4 : Insert and then Pop" << endl;
    PriorityQueueExperimentImpl<PriorityQueueTestImpl4>()();
    cout << endl << "Experiment 5 : Insert and then Pop (ordered)" << endl;
    PriorityQueueExperimentImpl<PriorityQueueTestImpl5>()();
}

