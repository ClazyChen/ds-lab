#include "vector.hpp"

using namespace dslab;

template <typename T>
using CyclicShift = Algorithm<void(Vector<T>&, std::size_t)>;

template <typename T>
class CyclicShiftStd : public CyclicShift<T> {
public:
    void operator()(Vector<T>& V, std::size_t k) override {
        std::rotate(V.begin(), V.begin() + k, V.end());
    }
    std::string type_name() const override {
        return "Cyclic Shift (std::rotate)";
    }
};

template <typename T>
class CyclicShiftMove : public CyclicShift<T> {
public:
    void operator()(Vector<T>& V, std::size_t k) override {
        Vector<T> W(k);
        std::move(V.begin(), V.begin() + k, W.begin());
        std::move(V.begin() + k, V.end(), V.begin());
        std::move(W.begin(), W.end(), V.end() - k);
    }
    std::string type_name() const override {
        return "Cyclic Shift (3 moves)";
    }
};

template <typename T>
class CyclicShiftSwap : public CyclicShift<T> {
    Gcd<std::size_t> gcd;
public:
    void operator()(Vector<T>& V, std::size_t k) override {
        auto d { gcd(V.size(), k) };
        for (auto i { 0uz }; i < d; ++i) {
            auto tmp { std::move(V[i]) };
            auto cur { i }, next { (cur + k) % V.size() };
            while (next != i) {
                V[cur] = std::move(V[next]);
                cur = next;
                next = (cur + k) % V.size();
            }
            V[cur] = std::move(tmp);
        }
    }
    std::string type_name() const override {
        return "Cyclic Shift (cyclic swap)";
    }
};

template <typename T>
class CyclicShiftReverse : public CyclicShift<T> {
public:
    void operator()(Vector<T>& V, std::size_t k) override {
        std::reverse(V.begin(), V.end());
        std::reverse(V.begin(), V.begin() + k);
        std::reverse(V.begin() + k, V.end());
    }
    std::string type_name() const override {
        return "Cyclic Shift (3 reverses)";
    }
};

std::vector<std::pair<std::size_t, std::size_t>> testData {
    {10, 2},
    {100'000'000, 1},
    {100'000'000, 10000},
    {100'000'000, 10007},
    {100'000'000, 50'000'000},
    {100'000'000, 90'000'000}
};

TestFramework<CyclicShift<std::size_t>,
    CyclicShiftStd<std::size_t>,
    CyclicShiftMove<std::size_t>,
    CyclicShiftSwap<std::size_t>,
    CyclicShiftReverse<std::size_t>> test;

int main() {
    for (auto [n, k] : testData) {
        std::cout << std::format("n = {:>10}, k = {:>10}", n, k) << std::endl;
        Vector<std::size_t> V(n);
        std::iota(V.begin(), V.end(), 0);
        test(std::ref(V), k);
    }
    return 0;
}