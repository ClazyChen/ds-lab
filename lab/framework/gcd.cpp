#include "framework.hpp"

using namespace dslab;

using GcdProblem = Algorithm<int(int, int)>;
using GcdEuclidIterative = Gcd;

class GcdBasic : public GcdProblem {
public:
    int operator()(int a, int b) override {
        int gcd { 0 };
        for (int i { 1 }; i <= a && i <= b; ++i) {
            if (a % i == 0 && b % i == 0) {
                gcd = i;
            }
        }
        return gcd;
    }
    std::string type_name() const override {
        return "Gcd (Naive)";
    }
};

// recursive Euclidean algorithm
// after compilation, it will be optimized into iterative form
// but we keep the recursive form for analysis in data structure
class GcdEuclid : public GcdProblem {
public:
    int operator()(int a, int b) override {
        if (b == 0) {
            return a;
        } else {
            return (*this)(b, a % b);
        }
    }
    std::string type_name() const override {
        return "Euclidean Gcd (Recursive)";
    }
};

// 中华更相减损术
class GcdCN : public GcdProblem {
public:
    int operator()(int a, int b) override {
        int p { 0 };
        while (a % 2 == 0 && b % 2 == 0) {
            a /= 2;
            b /= 2;
            ++p;
        }
        while (true) {
            while (a % 2 == 0) {
                a /= 2;
            }
            while (b % 2 == 0) {
                b /= 2;
            }
            if (a > b) {
                a -= b;
            } else if (a < b) {
                b -= a;
            } else {
                return a << p;
            }
        }
    }
    std::string type_name() const override {
        return "Gcd (Chinese)";
    }
};

std::vector<std::pair<int, int>> testData {
    {1, 1}, {1, 6},
    { 100'000'008, 12'345'678 },
    {24, 18}, {720, 1024},
//    {0, 0},   // UB
//    {0, 6},   // UB
//    {-8, 6},  // UB
//    {6, -8},  // UB
//    {-8, -8}, // UB
//    {-8, 4},  // UB
//    {8, -6}   // UB
};

TestFramework<GcdProblem, GcdBasic, GcdEuclid, GcdEuclidIterative, GcdCN> test;

int main() {
    for (auto& [a, b] : testData) {
        std::cout << std::format("Gcd({:>10}, {:>10})", a, b) << std::endl;
        test(a, b);
    }
}