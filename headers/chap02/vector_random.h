#pragma once

// 这个文件用于生成随机向量

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

    // 支持规定上下界
    Container apply(int n, double lo, double hi) {
        Container v(n);
        for (int i = 0; i < n; i++) {
            if constexpr (isInt) {
                v[i] = clazy_framework::Random::nextIntBetween((int)lo, (int)hi);
            } else if constexpr (isDouble) {
                v[i] = clazy_framework::Random::nextDoubleBetween(lo, hi);
            }
        }
        return v;
    }

    Container operator()(int n, double lo, double hi) {
        return apply(n, lo, hi);
    }

    Container operator()(int n, int lo, int hi) {
        return apply(n, lo, hi);
    }

    Container operator()(int n) {
        return apply(n);
    }
};

}