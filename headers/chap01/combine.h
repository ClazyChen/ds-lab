#pragma once

#include "framework.h"

// 这个文件考虑组合数的计算
// 经典的组合计数问题解决方案是递推+记忆化的做法

namespace clazy_framework {

class CombineProblem : public Algorithm<int, int, int> {

};

}

namespace clazy {

class Combine : public clazy_framework::CombineProblem {
private:
    vector<vector<int>> C; // 存储组合数的结果
public:
    int apply(int n, int m) override {
        m = min(m, n - m);                     // 利用对称性，降低一半的存储量
        if (n < C.size() && m < C[n].size()) { // 如果记忆化可用
            return C[n][m];                    // 直接读取记忆化的结果
        } else {                               // 如果不在记忆化的范围内则重新计算
            while (C.size() <= n) {
                C.push_back(vector<int> {1});  // 递推初始值
            }
            for (int i = C[n].size(); i <= m; i++) { // 进行递推
                C[n].push_back((long long)(n-i+1) * C[n].back() / i); // 为防止溢出使用long long
            }
            return C[n][m];
        }
    }
};

}
