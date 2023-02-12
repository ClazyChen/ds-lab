#include <vector>
#include <format>
#include <iostream>
import Stack;
import Framework;
import Combine;
using namespace dslab;
using namespace std;

class CombineRecursive1 : public CombineProblem {
public:
    int operator()(int n, int m) override {
        if (m == 0 || m == n)
            return 1;
        return (*this)(n - 1, m - 1) + (*this)(n - 1, m);
    }
};

class CombineRecursive2 : public CombineProblem {
    int sum { 0 };
    void combine(int n, int m) {
        if (m == 0 || m == n) {
            ++sum;
        } else {
            combine(n - 1, m - 1);
            combine(n - 1, m);
        }
    }
public:
    int operator()(int n, int m) override {
        sum = 0;
        combine(n, m);
        return sum;
    }
};

class CombineIterative : public CombineProblem {
public:
    int operator()(int n, int m) override {
        int sum { 0 };
        Stack<pair<int, int>> S { {n, m} };
        while (!S.empty()) {
            auto [n, m] { S.pop() };
            if (m == 0 || m == n) {
                ++sum;
            } else {
                S.push({ n - 1, m - 1 });
                S.push({ n - 1, m });
            }
        }
        return sum;
    }
};

class CombineMemorySearch : public CombineProblem {
    vector<vector<int>> C;
    void initialize(size_t n) {
        C.resize(n + 1);
        for (size_t i { 0 }; i <= n; ++i) {
            C[i].resize(i + 1, 0);
            C[i][0] = C[i][i] = 1;
        }
    }
public:
    int operator()(int n, int m) override {
        if (n >= C.size())
            initialize(n);
        if (C[n][m] == 0) {
            C[n][m] = (*this)(n - 1, m - 1) + (*this)(n - 1, m);
        }
        return C[n][m];
    }
};

vector<pair<int, int>> testData {
    {3, 2}, {6, 4}, {10, 5}, {20, 10}, {100, 4}
};

TestFramework<CombineProblem, CombineRecursive1, CombineRecursive2, CombineIterative, CombineMemorySearch> test;

int main() {
    for (auto& [n, m] : testData) {
        cout << format("n = {}, m = {}", n, m) << endl;
        test(n, m);
    }
}
