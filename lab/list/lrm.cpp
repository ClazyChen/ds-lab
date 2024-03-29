#include "vector.hpp"
#include "list.hpp"

using namespace dslab;

template <typename T>
class ContinuousPop : public Algorithm<void()> {
protected:
    List<T> L;
    Vector<typename List<T>::iterator> V;
public:
    virtual void initialize(std::size_t n) = 0;
    void operator()() override {
        for (auto p : V) {
            L.erase(p);
        }
    }
};

class SequentialPop : public ContinuousPop<std::size_t> {
public:
    void initialize(std::size_t n) override {
        L.clear();
        V.clear();
        for (auto i { 0uz }; i < n; ++i) {
            V.push_back(L.insert(L.end(), i));
        }
    }
    std::string type_name() const override {
        return "Sequential Pop";
    }
};

class RandomPop : public SequentialPop {
public:
    void initialize(std::size_t n) override {
        SequentialPop::initialize(n);
        std::shuffle(V.begin(), V.end(), Random::engine());
    }
    std::string type_name() const override {
        return "Random     Pop";
    }
};

Vector<std::size_t> testData { 10, 1000, 10000, 100'000, 1'000'000, 10'000'000 };

TestFramework<ContinuousPop<std::size_t>, RandomPop, SequentialPop> test;

int main() {
    for (auto n : testData) {
        std::cout << std::format("n = {:>10d}", n) << std::endl;
        test.run([n](auto& algo) { algo.initialize(n); });
        test();
    }
}