#include "stack.hpp"

using namespace dslab;

using CombProblem = Algorithm<int(int, int)>;

class CombRecursive2D : public CombProblem {
protected:
    int comb(int n, int k) {
        if (k == 0 || k == n) {
            return 1;
        } else {
            return comb(n - 1, k - 1) + comb(n - 1, k);
        }
    }
public:
    int operator()(int n, int k) override {
        return comb(n, k);
    }
    std::string type_name() const override {
        return "Comb (Recursive 2D)";
    }
};

class CombMemory2D : public CombProblem {
protected:
    std::vector<std::vector<int>> mem;
    int comb(int n, int k) {
        if (mem[n][k] == 0) {
            mem[n][k] = comb(n - 1, k - 1) + comb(n - 1, k);
        }
        return mem[n][k];
    }
public:
    int operator()(int n, int k) override {
        while (n >= mem.size()) {
            mem.push_back(std::vector<int>(mem.size() + 1, 0));
            mem.back()[0] = mem.back().back() = 1;
        }
        return comb(n, k);
    }
    std::string type_name() const override {
        return "Comb (Recursive Memory 2D)";
    }
};

class CombRollingArray : public CombProblem {
protected:
    std::vector<int> comb(int n) {
        if (n == 0) {
            return std::vector { 1 };
        } else {
            auto prev { comb(n - 1) };
            std::vector<int> ans(n + 1);
            ans[0] = ans[n] = 1;
            for (int i = 1; i < n; i++) {
                ans[i] = prev[i - 1] + prev[i];
            }
            return ans;
        }
    }
public:
    int operator()(int n, int k) override {
        return comb(n)[k];
    }
    std::string type_name() const override {
        return "Comb (Rolling Array)";
    }
};

class CombRecursive1D : public CombProblem {
protected:
    int comb(int n, int k) {
        if (k == 0) {
            return 1;
        }
        return comb(n, k - 1) * static_cast<std::size_t>(n - k + 1) / k;
    }
public:
    int operator()(int n, int k) override {
        return comb(n, k);
    }
    std::string type_name() const override {
        return "Comb (Recursive 1D)";
    }
};

std::vector<std::pair<int, int>> testData {
    {3, 2}, {6, 4}, {10, 5}, {20, 10}, {100, 4}
};

TestFramework<CombProblem, 
    CombRecursive2D, 
    CombMemory2D, 
    CombRollingArray, 
    CombRecursive1D,
    Comb<int>> test;

int main() {
    for (auto& [n, m] : testData) {
        std::cout << std::format("n = {}, m = {}", n, m) << std::endl;
        test(n, m);
    }
}