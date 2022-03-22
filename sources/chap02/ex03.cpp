#include "vector.h"
using namespace clazy_framework;

// 这个例子展示向量扩容和缩容的过程
// 当指定合理的缩容阈值时，向量可以通过扩容和缩容，保持自己的装填因子在一个受控的区间范围内

template <typename Allocator>
using Vector = clazy::Vector<int, Allocator>;

// 动态扩容和缩容的策略
template <int ExpandRatio, int ShrinkRatio, int ShrinkThreshold>
using DynamicVectorAllocator = clazy::RatioAllocator<ExpandRatio, ShrinkRatio, ShrinkThreshold>;

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