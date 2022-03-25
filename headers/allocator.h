#pragma once

#include "framework.h"

namespace clazy_framework {

// 向量容量策略
// 首先，向量需要支持扩容和缩容，因此需要考虑一个问题
// 输入：向量的当前容量capacity，目标规模size
//      扩容的场合，因为此时还没有扩容，所以size传入扩容之后的期待的目标规模
//      缩容的场合，size传入当前的规模即可
// 输出：向量是否需要扩容，目标容量
//      如果不需要扩容，目标容量保持原有的capacity

// 这个问题不单单是向量，也包括其他需要动态调整容量的场合
// 比如说，基于向量实现的队列，这里除了向量自身的扩容，还需要考虑队列的缩容（清理已经出队的元素留下的空间）
// 因此，单独提出来作为一个框架使用

// 容量分配模式
class AbstractAllocator : public Algorithm {
public:
    enum class Result {
        NoAction, Expand, Shrink
    };
protected:
    // 分成扩容和缩容两种情况，进行策略设计
    // 您可以默认扩容时capacity < size，缩容时capacity > size
    virtual pair<Result, int> expand(int capacity, int size) const = 0;
    virtual pair<Result, int> shrink(int capacity, int size) const = 0;
public:
    virtual pair<Result, int> apply(int capacity, int size) const {
        if (capacity < size) {            // 需要扩容
            return expand(capacity, size);
        } else if (capacity > size) {     // 需要缩容
            return shrink(capacity, size);
        } else /* capacity == size */ {   // 容量恰好等于规模的情况
            return {Result::NoAction, capacity};
        }
    }
};

}

namespace clazy {

// 默认的最小容量
constexpr const int default_min_capacity = 4;

// 经典的按比例扩容、缩容规则
// 扩容时加倍扩容
// 缩容时按比例缩容
// 装填因子低于某个阈值时，触发缩容
// 注意为了避免扩容和缩容之间左右横跳，阈值和加倍倍数需要被不等式限制
template <int ExpandRatio, int ShrinkRatio, int ShrinkThreshold = INT32_MAX, int MinCapacity = default_min_capacity> 
requires (ShrinkThreshold > ExpandRatio && ShrinkThreshold >= ShrinkRatio && ShrinkRatio >= 1 && ExpandRatio > 1 && MinCapacity > 0)
class RatioAllocator : public clazy_framework::AbstractAllocator {
protected:
    virtual pair<Result, int> expand(int capacity, int size) const override {
        while (capacity < size) {     // 加倍扩容策略
            capacity *= ExpandRatio;
        }
        return {Result::Expand, capacity};
    }
    virtual pair<Result, int> shrink(int capacity, int size) const override {
        if constexpr (ShrinkRatio > 1) { // 缩容倍率为1，表示永不缩容
            while (size < capacity / ShrinkThreshold && capacity > MinCapacity) { // 使用缩容阈值
                capacity /= ShrinkRatio; // 折半缩容策略
            }
            return {Result::Shrink, max(capacity, MinCapacity)}; 
        }
        return {Result::NoAction, capacity};
    }
};

}
