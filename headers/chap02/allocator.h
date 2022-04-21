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
        capacity *= ExpandRatio;               // 加倍扩容
        capacity = max(capacity, size);        // 如果加倍不够，直接扩容到和规模一致
        capacity = max(capacity, MinCapacity); // 规定最小容量
        return {Result::Expand, capacity};
    }
    virtual pair<Result, int> shrink(int capacity, int size) const override {
        if constexpr (ShrinkRatio > 1) { // 缩容倍率为1，表示永不缩容
            if (size < capacity / ShrinkThreshold) { // 装填因子低于阈值时，触发缩容
                capacity /= ShrinkRatio; // 按比例缩容
                capacity = min(capacity, size * ExpandRatio); // 如果缩容不够，则缩容到“刚扩容”的状态 
                capacity = max(capacity, MinCapacity); // 规定最小容量
                return {Result::Shrink, capacity};
            }
        }
        return {Result::NoAction, capacity};
    }
};

}
