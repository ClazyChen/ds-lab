#pragma once

#include "framework.h"

namespace clazy_framework {

// 这个文件考虑组合数的计算
class CombineProblem : public Algorithm {
protected:
    virtual int combine(int n, int m) = 0;
public:
    int apply(int n, int m) { return combine(n, m); }
};

}

namespace clazy {

// 经典的组合数递推+记忆化算法
class Combine : public clazy_framework::CombineProblem {
protected:
    vector<vector<int>> C;
    virtual int combine(int n, int m) {
        m = min(m, n - m);                     // 利用对称性，降低一半的存储量
        if (n < C.size() && m < C[n].size()) { // 如果记忆化可用
            return C[n][m];                    // 直接读取记忆化的结果
        } else {                               // 如果不在记忆化的范围内则重新计算
            while (C.size() <= n) {
                C.push_back(vector<int> {1});  // 递推初始值
            }
            for (int i : views::iota((int)C[n].size(), m+1)) { // 进行递推
                C[n].push_back((long long)(n-i+1) * C[n].back() / i); // 为防止溢出使用long long
            }
            return C[n][m];
        }
    }
};

}
