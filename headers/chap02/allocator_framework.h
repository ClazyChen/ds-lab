#pragma once

// 这个文件讨论的是向量容量策略
// 首先，向量需要支持扩容和缩容，因此需要考虑一个问题
// 输入：向量的当前容量capacity，目标规模size
//      扩容的场合，因为此时还没有扩容，所以size传入扩容之后的期待的目标规模
//      缩容的场合，size传入当前的规模即可
// 输出：向量是否需要扩容，目标容量
//      如果不需要扩容，目标容量保持原有的capacity

#include "framework.h"

namespace clazy_framework {

enum class AllocateResult {
    NoAction, Expand, Shrink
};

// 容量分配模式
class AbstractAllocator : public Algorithm<pair<AllocateResult, int>, int, int> {
public:
    using Result = AllocateResult;
protected:
    // 分成扩容和缩容两种情况，进行策略设计
    // 您可以默认扩容时capacity < size，缩容时capacity > size
    virtual pair<Result, int> expand(int capacity, int size) const = 0;
    virtual pair<Result, int> shrink(int capacity, int size) const = 0;
public:
    virtual pair<Result, int> apply(int capacity, int size) override {
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
