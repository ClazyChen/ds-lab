#include "framework.hpp"
#include <bit>

using namespace dslab;

using IsPower2 = Algorithm<bool(int)>;

class IsPower2Basic : public IsPower2 {
public:
    bool operator()(int n) override {
        return (n & (n - 1)) == 0;
    }
    std::string type_name() const override {
        return "IsPower2 (Discriminant)";
    }
};

class IsPower2SingleBit : public IsPower2 {
public:
    bool operator()(int n) override {
        return std::has_single_bit(static_cast<unsigned>(n));
    }
    std::string type_name() const override {
        return "IsPower2 (std::has_single_bit)";
    }
};

class IsPower2Recursive : public IsPower2 {
public:
    bool operator()(int n) override {
        if (n % 2 == 1) {
            return n == 1;
        } else {
            return (*this)(n / 2);
        }
    }
    std::string type_name() const override {
        return "IsPower2 (Recursive)";
    }
};

class IsPower2Iterative : public IsPower2 {
public:
    bool operator()(int n) override {
        int m { 1 };
        while (m < n) {
            m *= 2;
        }
        return m == n;
    }
    std::string type_name() const override {
        return "IsPower2 (Iterative)";
    }
};

std::vector testData {
    1, 2, 10, 256, 65536,
    0x80000, 0x88888, 0x8000000, 0x8000001
};

TestFramework<IsPower2, IsPower2Basic, IsPower2SingleBit, IsPower2Recursive, IsPower2Iterative> test;

int main() {
    for (auto n : testData) {
        std::cout << std::format("n = {:>10} : ", n) << std::endl;
        test(n);
    }
    return 0;
}