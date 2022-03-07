#pragma once

// 这个文件用于生成测试用的随机数据
#include <random>
#include <vector>
#include <ctime>
#include "vector.h"

namespace clazy_framework {

class Random {
private:
    static default_random_engine _random;
    static uniform_real_distribution<double> _doubleDist;
    static uniform_int_distribution<int> _intDist;
public:
    // 生成(0,1)中的随机数
    static double nextDouble();
    // 生成一个随机的16位无符号整数
    static unsigned int nextUInt();
    // 包括lo，不包括hi
    static int nextIntBetween(int lo, int hi);
};

double Random::nextDouble() {
    return _doubleDist(_random);
}

unsigned int Random::nextUInt() {
    return ((unsigned int)_intDist(_random)) << 16 | (unsigned int)_intDist(_random);
}

int Random::nextIntBetween(int lo, int hi) {
    int next = nextUInt() & 0x7fff'ffff;
    return next % (hi - lo) + lo;
}

default_random_engine Random::_random = default_random_engine(time(0));
uniform_real_distribution<double> Random::_doubleDist = uniform_real_distribution<double>(0.0, 1.0);
uniform_int_distribution<int> Random::_intDist = uniform_int_distribution<int>(0, 65535);

// 生成一个随机向量
template <typename Vector = clazy::Vector<int>>
requires (is_base_of_v<AbstractVector<int>, Vector>)
Vector randomVector(int n, int lo = 0, int hi = 65535) {
    Vector result(n);
    for (int i = 0; i < n; i++) {
        result.push_back(Random::nextIntBetween(lo, hi));
    }
    return result;
}

}