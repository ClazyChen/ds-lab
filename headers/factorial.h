#pragma once
#include "framework.h"

// 这个文件研究如何实现阶乘函数
namespace clazy_framework {

class FactorialProblem : public Algorithm {
public:
    virtual int apply(int n) const = 0;
};

}

namespace clazy {

// 经典的阶乘实现（打表）
class FactorialTable {
public:
    constexpr static const int max_number = 13;
    int table[max_number];
    constexpr int factorial(int n) {
        int result = 1;
        for (int i = 2; i <= n; i++) {
            result *= i;
        }
        return result;
    }
    constexpr FactorialTable(): table() {
        for (int i = 0; i < max_number; i++) {
            table[i] = factorial(i);
        }
    }
};

class Factorial : public clazy_framework::FactorialProblem {
protected:
    constexpr static FactorialTable table = FactorialTable();
public:
    virtual int apply(int n) const override {
        return table.table[n];
    }
};

}