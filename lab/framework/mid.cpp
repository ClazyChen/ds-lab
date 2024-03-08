#include "framework.hpp"
#include <limits>

using namespace dslab;
template <typename T>
using Mid = Algorithm<T(T, T)>;

template <typename T>
class Mid1 : public Mid<T> {
public:
    T operator()(T a, T b) override {
        return (a + b) / 2;
    }
    std::string type_name() const override {
        return "Mid (a + b) / 2";
    }
};

template <typename T>
class Mid2 : public Mid<T> {
public:
    T operator()(T a, T b) override {
        return a + (b - a) / 2;
    }
    std::string type_name() const override {
        return "Mid a + (b - a) / 2";
    }
};

template <typename T>
class Mid3 : public Mid<T> {
public:
    T operator()(T a, T b) override {
        return a / 2 + b / 2;
    }
    std::string type_name() const override {
        return "Mid a / 2 + b / 2";
    }
};

std::vector<std::pair<double, double>> ddata {
    { 0.0, 0.0 },
    { 0.0, 1.0 },
    { 0.0, std::numeric_limits<double>::min() },
    { std::numeric_limits<double>::min(), 1.0 },
    { std::numeric_limits<double>::min(), std::numeric_limits<double>::max() },
    { 0.0, std::numeric_limits<double>::max() },
    { -std::numeric_limits<double>::max(), 0.0 },
    { -std::numeric_limits<double>::max(), std::numeric_limits<double>::max() },
    { -std::numeric_limits<double>::max(), -std::numeric_limits<double>::max() },
};

std::vector<std::pair<int, int>> idata {
    { 0, 0 },
    { 0, 1 },
    { 0, std::numeric_limits<int>::min() },
    { std::numeric_limits<int>::min(), 1 },
    { std::numeric_limits<int>::min(), std::numeric_limits<int>::max() },
    { 0, std::numeric_limits<int>::max() },
    { -std::numeric_limits<int>::max(), 0 },
    { -std::numeric_limits<int>::max(), std::numeric_limits<int>::max() },
    { -std::numeric_limits<int>::max(), -std::numeric_limits<int>::max() },
};

TestFramework<Mid<double>, Mid1<double>, Mid2<double>, Mid3<double>> dtest;
TestFramework<Mid<int>, Mid1<int>, Mid2<int>, Mid3<int>> itest;

int main() {
    std::cout << "Double:" << std::endl;
    for (auto& [a, b] : ddata) {
        std::cout << std::format("Mid({:>10}, {:>10})\n", a, b);
        dtest(a, b);
    }
    std::cout << "Int:" << std::endl;
    for (auto& [a, b] : idata) {
        std::cout << std::format("Mid({:>10}, {:>10})\n", a, b);
        itest(a, b);
    }
    return 0;
}
