#include "framework.hpp"
#include <limits>

using namespace dslab;

using Sum = Algorithm<int(int)>;

class SumBasic : public Sum {
public:
    int operator()(int n) override {
        int sum { 0 };
        for (int i { 1 }; i <= n; ++i) {
            sum += i;
        }
        return sum;
    }
    std::string type_name() const override {
        return "Sum (Naive)";
    }
};

class SumAP : public Sum {
public:
    int operator()(int n) override {
        return n * (n + 1) / 2;
    }
    std::string type_name() const override {
        return "Sum (AP formula)";
    }
};

class SumAP2 : public Sum {
public:
    int operator()(int n) override {
        if (n % 2 == 0) {
            return n / 2 * (n + 1);
        } else {
            return (n + 1) / 2 * n;
        }
    }
    std::string type_name() const override {
        return "Sum (AP formula / Robust)";
    }
};

class SumAP2Limited : public SumAP2 {
    static constexpr int THRESHOLD { 0x0001'0000 };
public:
    int operator()(int n) override {
        if (n <= 0) {
            return 0;
        } else if (n > THRESHOLD) {
            return std::numeric_limits<int>::max();
        } else {
            return SumAP2::operator()(n);
        }
    }
    std::string type_name() const override {
        return "Sum (AP formula / Robust & limited)";
    }
};

std::vector testData {
    10    ,    // all correct
	0     ,    // all correct
    50'000,    // AP overflow, AP2 correct
    100'000,   // all overflow, but AP2Limited can return saturated value
    -10        // negative input, AP2Limited can return 0
};

// you can use the following code to test your implementation
TestFramework<Sum, SumBasic, SumAP, SumAP2, SumAP2Limited> test;

int main() {
	for (auto data : testData) {
        std::cout << std::format("Input: {}", data) << std::endl;
        test(data);
    }
    return 0;
}