#pragma once

#include "random_framework.h"
#include "vector_framework.h"

// 这个文件讨论如何随机生成一个合法的栈操作序列
// 这个算法来自高德纳《计算机程序设计艺术》第四卷，7.2.1.6
// 关于其设计思路，请参见卡塔兰数的扩展版本：卡塔兰三角的递推公式
// 它用来在栈这一部分生成随机测试样例

namespace clazy {

// 给定用来表示输入和输出的元素
template <typename T, typename Container = vector<T>>
requires (is_same_v<Container, vector<T>> || is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class RandomStackOperation : public clazy_framework::Algorithm<Container, int, const T&, const T&> {
public:
    Container apply(int n, const T& push, const T& pop) override {
        Container v(n * 2);
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

}