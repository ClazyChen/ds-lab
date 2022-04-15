#pragma once

// 这个文件描述了快速幂算法

#include "framework.h"

namespace clazy_framework {

// 求幂问题
// 输入：正整数a、b
// 输出：a的b次幂
class PowerProblem : public Algorithm<int, int, int> {

};

}

namespace clazy {

// 标准快速幂（迭代版本），时间O(logb)，空间O(1)
class Power : public clazy_framework::PowerProblem {
public:
    int apply(int a, int b) override {
        int ans = 1;            // 记录部分积
        while (b > 0) {         // 每次计算b的一位
            if (b % 2 == 1) { 
                ans *= a;
            }
            a *= a;
            b /= 2;
        }
        return ans;
    }
};

// 您可以用递降法证明上述算法的正确性

}