#include "vector.h"
#include "random.h"
#include "factorial.h"
#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示置乱的算法

template <typename T>
using Vector = clazy::Vector<T>;

// 置乱算法
template <typename T>
class Shuffle : public Algorithm<void, Vector<T>&>{
public:
    void apply(Vector<T>& V) override {
        for (int i = V.size() - 1; i > 0; i--) {
            swap(V[i], V[Random::nextIntBetween(0, i+1)]);
        }
    }
};

clazy::Factorial factorial;
Shuffle<int> shuffle_vector;

// 进行times次随机置乱
class ShuffleProblem : public Algorithm<void> {

};

template <int n> requires (n > 0)
class ShuffleTest : public ShuffleProblem {
private:
    Vector<int> V;

public:
    void clear() override {
        V.clear();
        V.resize(n);
        for (int i = 0; i < n; i++) {
            V[i] = i;
        }
    }

    void apply() override {
        shuffle_vector(V);
    }

    // 在这个实验中，还将验证随机打乱的等概率性
    // 当然，绝对等概率是不可能的，因为2^31的随机空间不能被n!(n>2)整除
    // 下面这个函数用于计算V的字典序序数
    int getFingerprint() {
        int result = 0;
        for (int i = 0; i < V.size(); i++) {
            result += (V[i] - count_if(V.begin(), V.begin() + i, [&](int x) { return x < V[i]; })) \
                    * factorial(V.size() - 1 - i);
        }
        return result;
    }
};

// 第一个实验，测试置乱算法的效率
void experiment1() {
    TestFramework<ShuffleProblem, 
        ShuffleTest<10>, 
        ShuffleTest<1000>, 
        ShuffleTest<100'000>, 
        ShuffleTest<1'000'000>, 
        ShuffleTest<10'000'000>> tf;
    tf.clear();
    tf.test();
}

// 第二个实验，测试置乱算法的等概率性
void experiment2() {
    constexpr static int n = 4;
    constexpr static int shuffle_number = 10'000'000;
    ShuffleTest<n> test;
    test.clear();
    int permutation = factorial(n);
    Vector<int> counter(permutation);
    fill(begin(counter), end(counter), 0);
    for (int i = 0; i < shuffle_number; i++) {
        test.apply();
        int fingerprint = test.getFingerprint();
        counter[fingerprint]++;
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
}

int main() {
    experiment1();
    experiment2();
    return 0;
}
