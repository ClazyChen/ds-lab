import Framework;
import Vector;
import PriorityQueue.WinnerTree;
import std;

using namespace dslab;
using namespace std;

template <typename T>
class BuildWinnerTree : public Algorithm<T()> {
protected:
    Vector<T> V;
public:
    void initialize(const Vector<T>& V) {
        this->V = V;
    }
};

template <typename T>
class BuildWinnerTreeLocal : public BuildWinnerTree<T> {
public:
    T operator()() override {
        WinnerTree<T> W;
        for (auto e : this->V) {
            W.push(e);
        }
        return W.top();
    }
    string type_name() const override {
        return "Build Winner Tree / Insert one by one";
    }
};

template <typename T>
class BuildWinnerTreeGlobal : public BuildWinnerTree<T> {
public:
    T operator()() override {
        return WinnerTree<T>(move(this->V)).top();
    }
    string type_name() const override {
        return "Build Winner Tree / Global";
    }
};

default_random_engine engine { random_device{}() };
vector testData { 10, 100, 1000, 10000, 100000, 1'000'000 };

TestFramework<BuildWinnerTree<int>,
    BuildWinnerTreeLocal<int>,
    BuildWinnerTreeGlobal<int>
> test;

template <bool enable_shuffle >
void runTests() {
    for (auto n : testData) {
        cout << format("n = {:>8}", n) << endl;
        Vector<int> V(n);
        iota(begin(V), end(V), 0);
        if constexpr (enable_shuffle) {
            shuffle(begin(V), end(V), engine);
        } else {
            reverse(begin(V), end(V));
        }
        test.run([&V](auto& algo) { algo.initialize(V); });
        test();
    }
}

int main() {
    cout << "Random Test" << endl;
    runTests<true>();
    cout << "Reverse (Worst) Test" << endl;
    runTests<false>();
}