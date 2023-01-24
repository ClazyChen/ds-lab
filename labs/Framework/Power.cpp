#include <vector>
#include <format>
#include <iostream>
import Framework;
import Power;
using namespace dslab;
using namespace std;

// 求幂问题
// 基类参见Power.ixx

// 直接计算的方法
class PowerBasic : public PowerProblem {
public:
    int operator()(int a, int b) override {
        int result { 1 };
        for (int i { 0 }; i < b; ++i) {
            result *= a;
        }
        return result;
    }
};

// 快速幂（递归版本）
class PowerFastRecursive : public PowerProblem {
public:
    int operator()(int a, int b) override {
        if (b == 0) {
            return 1;
        } else if (b % 2 == 1) {
            return a * (*this)(a * a, b / 2);
        } else {
            return (*this)(a * a, b / 2);
        }
    }
};

using PowerFastIterative = Power;

// 测试数据
vector<pair<int, int>> testData {
    {3, 4}, {2, 30}, {-3, 3}, // 一些常规问题
    {1, 1'000'000'000} // 当b很大时，效率差异明显
};

// 可以将您的实现加入到测试框架的参数列表中
TestFramework<PowerProblem, PowerBasic, PowerFastRecursive, PowerFastIterative> test;

int main() {
    for (auto& [a, b] : testData) {
        cout << format("a={}, b={}", a, b) << endl;
        test(a, b);
    }
    return 0;
}