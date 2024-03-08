#include "framework.hpp"

using namespace dslab;

using PowerProblem = Algorithm<int(int, int)>;

class PowerBasic : public PowerProblem {
public:
    int operator()(int a, int b) override {
        int result { 1 };
        for (int i { 0 }; i < b; ++i) {
            result *= a;
        }
        return result;
    }
    std::string type_name() const override {
        return "Power (naive)";
    }
};

// recursive
class PowerRecursive : public PowerProblem {
public:
    int operator()(int a, int b) override {
        if (b == 0) {
            return 1;
        } else if (b % 2 == 0) {
            return (*this)(a * a, b / 2);
        } else {
            return a * (*this)(a * a, b / 2);
        }
    }
    std::string type_name() const override {
        return "Exponent by square (Recursive)";
    }
};

// iterative
using PowerIterative = Power;

std::vector<std::pair<int, int>> testData {
    {3, 4}, {2, 30}, {-3, 3}, // some common cases
    {3, 1'000'000'000} // when b is large, the difference is obvious
};

TestFramework<PowerProblem, PowerBasic, PowerRecursive, PowerIterative> test;

int main() {
    for (auto& [a, b] : testData) {
        std::cout << std::format("a = {:>10}, b = {:>10}", a, b) << std::endl;
        test(a, b);
    }
    return 0;
}