#pragma once

#include "framework.h"

namespace clazy_framework {

// 最大公约数问题
// 输入：两个整数a, b
// 输出：a和b的最大公约数
class GcdProblem : public Algorithm {
protected:
    virtual int gcd(int a, int b) const = 0;
public:
    int apply(int a, int b) const { return gcd(a, b); }
};

}

namespace clazy {

// 经典的欧几里得辗转相除法（迭代版本）
class Gcd : public clazy_framework::GcdProblem {
protected:
    int gcd(int a, int b) const {
        while (b > 0) {
            int temp = a % b;
            a = b, b = temp;
        }
        return a;
    }
};

}