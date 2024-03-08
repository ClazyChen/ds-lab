#include "framework.hpp"
#include <cmath>

using namespace dslab;

// the example of integral
int f(int n) {
    int result = 0;
    for (int i { 1 }; i <= n; ++i) {
        for (int j { 1 }; j <= i; ++j)
            for (int k { 1 }; k <= j; ++k)
                for (int l { 1 }; l <= j; l *= 2)
                    result += k * l;
    }
    return result;
}

std::vector<int> testData {
    5, 10, 30, 100, 250, 400, 700, 800, 900, 1000
};

int main() {
    for (auto n : testData) {
        auto [result, time] { reportTime<int>([&] { return f(n); }) };
        auto relative { time * 1e9 / (n * n * n * std::log2(n)) };
        std::cout << std::format("f({:>4}) = {:>12} ({:.6f}s, {:.6f}ns/n^3logn)", n, result, time, relative) << std::endl;
    }
}