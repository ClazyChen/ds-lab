#include "vector.h"
#include "random.h"
#include "factorial.h"
#include <cmath>
using namespace clazy_framework;

// 这个例子展示置乱的算法

template <typename T>
using Vector = clazy::Vector<T>;

// 全局随机数发生器
Random random;

// 置乱算法
template <typename T>
class Shuffle : public Algorithm {
public:
    void apply(Vector<T>& V) {
        for (Rank i = V.size(); i > 0; i--) {
            swap(V[i-1], V[random.nextIntBetween(0, i)]);
        }
    }
};

// 在这个实验中，将验证随机打乱的等概率性
// 当然，绝对等概率是不可能的，因为2^31的随机空间不能被n!(n>2)整除
// 下面这个函数用于计算V的字典序序数
clazy::Factorial factorial;
int getFingerprint(const Vector<int>& V) {
    int result = 0;
    for (int i = 0; i < V.size(); i++) {
        result += (V[i] - count_if(begin(V), begin(V) + i, [&](int x) { return x < V[i]; })) * factorial.apply(V.size() - 1 - i);     
    }
    return result;
}

const int n              = 4;            // 进行随机打乱的向量规模
const int shuffle_number = 100'000'000;  // 进行随机打乱的次数

int main() {
    Vector<int> V;
    for (int i = 0; i < n; i++) {
        V.push_back(i);
    }
    auto algorithm = make_shared<Shuffle<int>>();
    // 首先进行时间实验，这里不做统计
    applyTest<Shuffle<int>>(algorithm, [&](auto shuffle) {
        cout << "shuffling timing test ... ";
        for (int i = 0; i < shuffle_number; i++) {
            shuffle->apply(V);
        }
    });
    // 其次进行统计实验，观察随机打乱的近似等概率性
    int permutation = factorial.apply(n);
    Vector<int> counter(permutation);
    counter.resize(permutation);
    fill(begin(counter), end(counter), 0);
    cout << "shuffling result test ... " << endl;
    for (int i = 0; i < shuffle_number; i++) {
        algorithm->apply(V);
        counter[getFingerprint(V)] ++;
    }
    cout << "statistics: " << counter << endl;
    double average = (double)shuffle_number / permutation;
    // 各种排列的命中频率，和平均值之间的相对误差
    double bias = 0.0;
    for (int c : counter) {
        bias += abs(average - c)/ average;
    }
    // 平均的相对误差应当是非常小的
    cout << "average bias: " << bias / permutation  << endl;
    return 0;
}
