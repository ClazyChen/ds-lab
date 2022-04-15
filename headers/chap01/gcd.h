#pragma once

#include "framework.h"

namespace clazy_framework {

// 最大公约数问题
// 输入：两个整数a, b
// 输出：a和b的最大公约数
class GcdProblem : public Algorithm<int, int, int> {

};

}

namespace clazy {

// 经典的欧几里得辗转相除法（迭代版本）
class Gcd : public clazy_framework::GcdProblem {
public:
    int apply(int a, int b) override {
        while (b != 0) {
            int t = b;
            b = a % b;
            a = t;
        }
        return a;
    }
};

}