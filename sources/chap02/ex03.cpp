#include "vector.h"
using namespace clazy_framework;

// 这个例子展示向量扩容和缩容的过程
// 当指定合理的缩容阈值时，向量可以通过扩容和缩容，保持自己的装填因子在一个受控的区间范围内

template <typename Allocator> requires (is_base_of_v<VectorAllocator, Allocator>)
using Vector = clazy::Vector<int, Allocator>;

// 采用加倍扩容策略，同时增加缩容策略
// 装填因子低于某个阈值时触发减半缩容
// 注意为了避免扩容和缩容之间左右横跳，阈值和加倍倍数有不等式限制
template <int ExpandRatio, int ShrinkRatio, int ShrinkThreshold> requires (ShrinkThreshold > ExpandRatio)
class DynamicVectorAllocator : public VectorAllocator {
protected:
    virtual pair<Result, int> expand(int capacity, int size) const {
        while (capacity < size) {     // 加倍扩容策略
            capacity *= ExpandRatio;
        }
        return {Result::Expand, capacity};
    }
    virtual pair<Result, int> shrink(int capacity, int size) const {
        if (size < capacity / ShrinkThreshold && capacity > clazy::min_vector_capacity) {    // 设置缩容阈值
            return {Result::Shrink, max(capacity / ShrinkRatio, clazy::min_vector_capacity)};
        } else {
            return {Result::NoAction, capacity};
        }
    }
};

const int op_number = 9;   // 进行插入、删除的次数
const int expand_ratio = 2;
const int shrink_ratio = 2;
const int shrink_threshold = 4;

int main() {
    Vector<DynamicVectorAllocator<expand_ratio, shrink_ratio, shrink_threshold>> V;
    for (int i : views::iota(0, op_number)) {
        V.push_back(i);
        cout << "inserted capacity: " << V.capacity() << "\t V = " << V << endl;
    }
    for (int i : views::iota(0, op_number)) {
        V.pop_back();
        cout << "removed  capacity: " << V.capacity() << "\t V = " << V << endl;
    }
}