#include <vector>
#include <format>
#include <iostream>
import Framework;
import Vector;
using namespace dslab;
using namespace std;

class VectorConcat : public Algorithm<Vector<int>&(Rank)> {
protected:
    Vector<int> V, V1;
public:
    void initialize(int n, int n1) {
        V.reserve(static_cast<size_t>(n) + n1);
        V1.reserve(n1);
        V.resize(n, 0);
        V1.resize(n1, 1);
    }
};

class VectorConcatBasic : public VectorConcat {
public:
    Vector<int>& operator()(Rank r) override {
        for (int i : V1) {
            V.insert(r, i);
            ++r;
        }
        return V;
    }
};

class VectorConcatFast : public VectorConcat {
public:
    Vector<int>& operator()(Rank r) override {
        V.resize(V.size() + V1.size());
        std::move_backward(V.begin() + r, V.end() - V1.size(), V.end());
        std::move(V1.begin(), V1.end(), V.begin() + r);
        return V;
    }
};

vector<tuple<size_t, size_t, Rank>> testData {
    {100000, 100, 0},         // 插入少量元素到开头
    {100000, 100000, 0},      // 插入大量元素到开头
    {100000, 100, 100000},    // 插入少量元素到结尾
    {100000, 100000, 100000}  // 插入大量元素到结尾
};

TestFramework<VectorConcat, VectorConcatBasic, VectorConcatFast> test;

int main() {
    for (auto& [n, n1, r] : testData) {
        cout << format("n = {}, n1 = {}, r = {}", n, n1, r) << endl;
        test.run([n, n1](auto& algo) { algo.initialize(n, n1); });
        test(r);
    }
}

