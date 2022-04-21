#include "vector.h"
#include "gcd.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子讨论一个有趣的话题：循环位移
// 输入：向量V，位移量k
// 输出：将向量V循环地左移k个长度

template <typename T>
using Vector = clazy::Vector<T>;

using Gcd = clazy::Gcd;

template <typename T>
class CyclicLeftShift : public Algorithm<void, Vector<T>&, int> {

};

// 这个算法应当有O(n)的时间复杂度

// 朴素的想法（三次赋值，数组拷贝）
// 这种方法和经典的三次赋值swap很相似
template <typename T>
class CyclicLeftShiftSwap : public CyclicLeftShift<T> {
public:
    void apply(Vector<T>& V, int k) override {
        auto B = make_unique<T[]>(k); // 辅助空间
        copy(V.begin(), V.begin() + k, B.get());   // B[0:k] = V[0:k]
        copy(V.begin() + k, V.end(), V.begin());    // V[0:n-k] = V[k:n]
        copy(B.get(), B.get() + k, V.end() - k);  // V[n-k:n] = B[0:k]
    }
};

// 遍历环交换
template <typename T>
class CyclicLeftShiftRing : public CyclicLeftShift<T> {
private:
    Gcd gcd;
public:
    void apply(Vector<T>& V, int k) override {
        int n = V.size();
        int d = gcd(n, k);              // 计算最大公约数
        T temp;                         // 辅助空间，只需要1个（循环交换）
        for (int i = 0; i < d; i++) {
            temp = V[i];
            int j = i, next_j;
            while (next_j = (j + k) % n, next_j != i) {
                V[j] = V[next_j];
                j = next_j;
            }                           // 以k为步长位移，遍历整个环    
            V[j] = temp;                // 最后一步，从辅助空间取出
        }
    }
};

// 三次颠倒
template <typename T>
class CyclicLeftShiftReverse : public CyclicLeftShift<T> {
public:
    void apply(Vector<T>& V, int k) override {
        reverse(V.begin(), V.begin() + k); // -> rV[0:k] + V[k:n]
        reverse(V.begin(), V.end());       // -> rV[k:n] + V[0:k]
        reverse(V.begin(), V.end() - k);   // -> V[k:n] + V[0:k]
    }
};

pair<int, int> testData[] {
    {10, 2},
    {10'000'000, 1},         // swap是非常典型的处理方案，和普通的swap
    {10'000'000, 100'00},    // reverse和swap的效率相仿，并且不需要额外的空间
    {10'000'000, 100'07},    // ring方法看似对数组元素的赋值次数最少（一步到位），但计算复杂且跳跃，反而最慢
    {10'000'000, 5'000'000}, // ring的效率，不但和k有关，还和(n,k)的最大公约数有关
    {10'000'000, 9'000'000}, // 这个问题作出解释，需要《组成原理》的知识
    {100'000'000, 1},        // 简单地说，swap和reverse都充分利用了局部性（从而可以很好的吃Cache和TLB）
    {100'000'000, 100'000},  // 而ring是跳跃赋值的，所以会很慢
    {100'000'000, 100'007},
    {100'000'000, 50'000'000},
    {100'000'000, 90'000'000}
};

int main() {
    TestFramework<CyclicLeftShift<int>,
        CyclicLeftShiftSwap<int>,
        CyclicLeftShiftRing<int>,
        CyclicLeftShiftReverse<int>> tf;
    for (auto [n, k] : testData) {
        cout << "Testing n = " << n << " k = " << k << endl;
        Vector<int> V(n);
        for (int i = 0; i < n; i++) {
            V[i] = i;
        }
        random_shuffle(V.begin(), V.end());  // 生成实验数据
        tf.test(V, k);
    }
    return 0;
}
