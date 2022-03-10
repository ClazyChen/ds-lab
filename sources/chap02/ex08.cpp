#include "vector.h"
#include "gcd.h"
#include <cassert>
using namespace clazy_framework;

// 这个例子讨论一个有趣的话题：循环位移
// 输入：向量V，位移量k
// 输出：将向量V循环地左移k个长度

template <typename T>
using Vector = clazy::Vector<T>;

using Gcd = clazy::Gcd;

template <typename T>
class CyclicLeftShift : public Algorithm {
public:
    virtual void apply(Vector<T>& V, int k) = 0;
};

// 这个算法应当有O(n)的时间复杂度

// 朴素的想法（三次赋值，数组拷贝）
// 这种方法和经典的三次赋值swap很相似
template <typename T>
class CyclicLeftShiftSwap : public CyclicLeftShift<T> {
public:
    virtual void apply(Vector<T>& V, int k) {
        auto B = make_unique<T[]>(k); // 辅助空间
        copy(begin(V), begin(V) + k, B.get());   // B[0:k] = V[0:k]
        copy(begin(V) + k, end(V), begin(V));    // V[0:n-k] = V[k:n]
        copy(B.get(), B.get() + k, end(V) - k);  // V[n-k:n] = B[0:k]
    }
};

// 遍历环交换
template <typename T>
class CyclicLeftShiftRing : public CyclicLeftShift<T> {
protected:
    shared_ptr<GcdProblem> gcd;
public:
    CyclicLeftShiftRing(): gcd(make_shared<Gcd>()) {}
    virtual void apply(Vector<T>& V, int k) {
        int n = V.size();
        int d = gcd->apply(n, k);       // 计算最大公约数
        T temp;                         // 辅助空间，只需要1个（循环交换）
        for (Rank i = 0; i < d; i++) {
            temp = V[i];
            Rank j = i, next_j = (j + k) % n;
            do {
                V[j] = V[next_j];
                j = next_j;
                next_j = (j + k) % n;
            } while (next_j != i);      // 以k为步长位移，遍历整个环
            V[j] = temp;                // 最后一步，从辅助空间取出
        }
    }
};

// 三次颠倒
template <typename T>
class CyclicLeftShiftReverse : public CyclicLeftShift<T> {
public:
    virtual void apply(Vector<T>& V, int k) {
        reverse(begin(V), begin(V) + k); // -> rV[0:k] + V[k:n]
        reverse(begin(V), end(V));       // -> rV[k:n] + V[0:k]
        reverse(begin(V), end(V) - k);   // -> V[k:n] + V[0:k]
    }
};

int main() {
    auto algorithms = generateInstances<CyclicLeftShift<int>, CyclicLeftShiftSwap<int>, CyclicLeftShiftRing<int>, CyclicLeftShiftReverse<int>>();
    pair<int, int> testData[] {
        {10, 2},                 // swap方法的时间复杂度是Theta(n+k)，结果和k有关
        {10'000'000, 1},         // k较小时比reverse快，k较大时比reverse慢
        {10'000'000, 100'00},    // reverse对于k的影响不大
        {10'000'000, 100'07},    // 而ring方法，看似对数组元素的赋值次数最少（一步到位），但计算复杂且跳跃，反而最慢
        {10'000'000, 5'000'000}, // ring的效率，不但k有关，还和(n,k)的最大公约数有关
        {10'000'000, 9'000'000}, // 这个问题作出解释，需要《组成原理》的知识
        {100'000'000, 1},        // 简单地说，swap和reverse都充分利用了局部性（从而可以很好的吃Cache和TLB）
        {100'000'000, 100'000},  // 而ring是跳跃赋值的，所以会很慢
        {100'000'000, 100'007},
        {100'000'000, 50'000'000},
        {100'000'000, 90'000'000}
    };
    for (auto [n, k] : testData) {
        cout << "Testing n = " << n << " k = " << k << endl;
        Vector<int> V(n);
        for (int i = 0; i < n; i++) {
            V.push_back(i);
        }
        random_shuffle(begin(V), end(V));  // 生成实验数据
        for (auto algorithm : algorithms) {
            auto V_t = V; // 进行移位的向量
            applyTest<CyclicLeftShift<int>>(algorithm, [&](auto cyclicLeftShift) {
                cyclicLeftShift->apply(V_t, k);
            });
            for (int i = 0; i < n; i++) {
                assert(V_t[i] == V[(i+k)%n]);
            }
        }
    }
    return 0;
}
