#include <vector>
#include <format>
#include <iostream>
import Framework;
import Power;
using namespace dslab;
using namespace std;

// 一个示例函数
// 在这个函数中，我们可以看到，它的复杂度是O(n^3 * log(n))
// 请不要开启O2优化，否则编译器优化会影响到我们的测试结果
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

vector testData {
    5, 10, 30, 100, 250, 400, 700, 800, 900, 1000
};

int main() {
    for (auto n : testData) {
        auto [result, time] = reportTime<int>([&] { return f(n); });
        auto relative = time * 1e9 / (n * n * n * log2(n));
        cout << format("f({:>4}) = {:>12} ({:.6f}s, {:.6f}ns/n^3logn)", n, result, time, relative) << endl;
    }
}

