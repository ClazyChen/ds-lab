#pragma once

// 这个文件给出了一种通用的扩容、缩容策略

#include "allocator_framework.h"

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
