import Framework;
import Vector;
import Gcd;
import std;

using namespace dslab;
using namespace std;

template <typename T>
class CyclicShift : public Algorithm<void(Vector<T>&, size_t)> {};

template <typename T>
class CyclicShiftStd : public CyclicShift<T> {
public:
    void operator()(Vector<T>& V, size_t k) override {
        rotate(begin(V), begin(V) + k, end(V));
    }
    string type_name() const override {
        return "Cyclic Shift (std::rotate)";
    }
};

template <typename T>
class CyclicShiftMove : public CyclicShift<T> {
public:
    void operator()(Vector<T>& V, size_t k) override {
        Vector<T> W(k);
        move(begin(V), begin(V) + k, begin(W));
        move(begin(V) + k, end(V), begin(V));
        move(begin(W), end(W), end(V) - k);
    }
    string type_name() const override {
        return "Cyclic Shift (3 moves)";
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
    string type_name() const override {
        return "Cyclic Shift (cyclic swap)";
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
    string type_name() const override {
        return "Cyclic Shift (3 reverses)";
    }
};

vector<pair<int, int>> testData {
    {10, 2},
    {100'000'000, 1},
    {100'000'000, 10000},
    {100'000'000, 10007},
    {100'000'000, 50'000'000},
    {100'000'000, 90'000'000}
};

TestFramework<CyclicShift<int>, CyclicShiftStd<int>, CyclicShiftMove<int>, CyclicShiftSwap<int>, CyclicShiftReverse<int>> test;

int main() {
    for (auto& [n, k] : testData) {
        cout << format("n = {:>10}, k = {:>10}", n, k) << endl;
        Vector<int> V(n);
        iota(begin(V), end(V), 0);
        test(ref(V), k);
    }
}