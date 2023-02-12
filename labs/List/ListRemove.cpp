#include <vector>
#include <format>
#include <random>
#include <iostream>
import Framework;
import Vector;
import List;
using namespace dslab;
using namespace std;

template <typename T>
class ContinuousPop : public Algorithm<void()> {
protected:
    List<T> L;
    Vector<ListNode<T>*> V;
public:
    virtual void initialize(size_t n) = 0;
    void operator()() override {
        for (auto&& p : V) {
            L.remove(p);
        }
    }
};

class SequentialPop : public ContinuousPop<int> {
public:
    void initialize(size_t n) override {
        L.clear();
        V.clear();
        for (size_t i { 0 }; i < n; ++i) {
            V.push_back(L.insertAsNext(L.last(), i));
        }
    }
};

class RandomPop : public SequentialPop {
    default_random_engine m_engine { random_device {}() };
public:
    void initialize(size_t n) override {
        SequentialPop::initialize(n);
        shuffle(V.begin(), V.end(), m_engine);
    }
};

vector testData { 10, 1000, 10000, 100'000, 1'000'000, 10'000'000 };

TestFramework<ContinuousPop<int>, RandomPop, SequentialPop> test;

int main() {
    for (auto n : testData) {
        cout << format("n = {:10d}", n) << endl;
        test.run([n](auto& algo) { algo.initialize(n); });
        test();
    }
}