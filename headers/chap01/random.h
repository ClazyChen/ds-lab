#pragma once

// 这个文件是对random_framework的简单扩展
// 实现了一些特定的随机生成算法
// 1. 随机生成向量
// 2. 随机生成合法的栈操作序列

#include "random_framework.h"
#include "vector_framework.h"

namespace clazy {

// 给定向量的长度
// 生成一个随机的向量
// 1. 如果给定的参数是int，则生成随机的0~65535
// 2. 如果给定的参数是double，则生成随机的0~1
template <typename T = int, typename Container = vector<T>>
requires ((is_same_v<T, int> || is_same_v<T, double>) && 
          (is_same_v<Container, vector<T>> || is_base_of_v<clazy_framework::AbstractVector<T>, Container>))
class RandomVector : public clazy_framework::Algorithm<Container, int> {
private:
    constexpr static bool isInt = is_same_v<T, int>;
    constexpr static bool isDouble = is_same_v<T, double>;
public:
    Container apply(int n) override {
        Container v(n);
        for (int i = 0; i < n; i++) {
            if constexpr (isInt) {
                v[i] = clazy_framework::Random::nextUInt16();
            } else if constexpr (isDouble) {
                v[i] = clazy_framework::Random::nextDouble();
            }
        }
        return v;
    }
};

// 生成一个随机的合法栈操作序列
// 给定用来表示输入和输出的元素
// 这个算法来自高德纳《计算机程序设计艺术》第四卷，7.2.1.6
// 关于其设计思路，请参见卡塔兰数的扩展版本：卡塔兰三角的递推公式
// 它用来在栈这一部分生成随机测试样例
template <typename T>
class RandomStackOperation : public clazy_framework::Algorithm<vector<T>, int, const T&, const T&> {
public:
    vector<T> apply(int n, const T& push, const T& pop) override {
        vector<T> v(n);
        int p = n, q = n;
        for (int i = 0; i < n * 2; i++) {
            int r = clazy_framework::Random::nextIntBetween(0, (p+q) * (q-p+1));
            if (r < p * (q-p+2)) {
                v[i] = push;
                p--;
            } else {
                v[i] = pop;
                q--;
            }
        }
        return v;
    }
};

// // 生成一个随机的栈操作序列
// template <typename T, typename Vector = clazy::Vector<T>>
// requires (is_base_of_v<AbstractVector<T>, Vector>)
// Vector randomStackOperation(int n, T push_e, T pop_e) {
//     int p = n, q = n;
//     Vector V;
//     for (int i : views::iota(0, n * 2)) {
//         int r = Random::nextIntBetween(0, (p+q) * (q-p+1));
//         if (r < p * (q-p+2)) {
//             V.push_back(push_e); p--;
//         } else {
//             V.push_back(pop_e); q--;
//         }
//     }
//     return V;
// }

}
