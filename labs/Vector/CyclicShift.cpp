#include <vector>
#include <format>
#include <numeric>
#include <iostream>
import Framework;
import Vector;
import Gcd;
using namespace dslab;
using namespace std;

template <typename T>
class CyclicShift : public Algorithm<void, Vector<T>&, size_t> {};

template <typename T>
class CyclicShiftMove : public CyclicShift<T> {
public:
    void operator()(Vector<T>& V, size_t k) override {
        Vector<T> W(k);
        move(begin(V), begin(V) + k, begin(W));
        move(begin(V) + k, end(V), begin(V));
        move(begin(W), end(W), end(V) - k);
    }
};

template <typename T>
class CyclicShiftSwap : public CyclicShift<T> {
    Gcd gcd;
public:
    void operator()(Vector<T>& V, size_t k) override {
        size_t d { static_cast<size_t>(gcd(V.size(), k)) };
        for (Rank i { 0 }; i < d; ++i) {
            T tmp { move(V[i]) };
            Rank cur { i }, next { (cur + k) % V.size() };
            while (next != i) {
                V[cur] = move(V[next]);
                cur = next;
                next = (cur + k) % V.size();
            }
            V[cur] = move(tmp);
        }
    }
};

template <typename T>
class CyclicShiftReverse : public CyclicShift<T> {
public:
    void operator()(Vector<T>& V, size_t k) override {
        reverse(begin(V), end(V));
        reverse(begin(V), end(V) - k);
        reverse(end(V) - k, end(V));
    }
};

vector<pair<int, int>> testData {
    {10, 2},
    {10'000'000, 1},
    {10'000'000, 10000},
    {10'000'000, 10007},
    {10'000'000, 5'000'000},
    {10'000'000, 9'000'000}
};

TestFramework<CyclicShift<int>, CyclicShiftMove<int>, CyclicShiftSwap<int>, CyclicShiftReverse<int>> test;

int main() {
    for (auto& [n, k] : testData) {
        cout << format("n = {:10}, k = {:10}", n, k) << endl;
        Vector<int> V(n);
        iota(begin(V), end(V), 0);
        test(ref(V), k);
    }
}