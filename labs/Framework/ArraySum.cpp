import Framework;
import std;

using namespace dslab;
using namespace std;

// 数组求和问题
class ArraySum : public Algorithm<int(span<const int>)> {};

// std::span是C++20引入的视图
// 可以对C-style array、std::array、std::vector等进行视图化
// 从而可以使用STL算法对其进行操作
// 它的本质是一个指针和长度的结构体，具有轻量级的特点
// 因此，我们可以用一个很低的复制成本，对它进行值传递

// 需要注意的是，std::span仅仅是一个视图，所以const span<T>和span<const T>是不同的
// 如果要求span内部的元素不可修改，应该使用span<const T>，而不是const span<T>

// 普通方法求和，用于验证正确性
// 这里使用了STL提供的求和函数std::accumulate和std::reduce
class ArraySumBasic : public ArraySum {
public:
    int operator()(span<const int> data) override {
        return accumulate(begin(data), end(data), 0);
    }
    string type_name() const override {
        return "ArraySum (std::accumulate)";
    }
};

class ArraySumReduce : public ArraySum {
public:
    int operator()(span<const int> data) override {
        return reduce(begin(data), end(data), 0);
    }
    string type_name() const override {
        return "ArraySum (std::reduce)";
    }
};

// 不使用STL的求和函数，使用朴素的迭代法求和
// 可以看出，std::reduce更接近于直接求和的优化结果
// 另一方面，std::accumulate则阻碍了编译器的优化，以面向更加通用（不符合结合律）的操作
class ArraySumIterative : public ArraySum {
public:
    int operator()(span<const int> data) override {
        int sum { 0 };
        for (auto x : data) {
            sum += x;
        }
        return sum;
    }
    string type_name() const override {
        return "ArraySum (Iterative)";
    }
};

// 减治法求和，先算前n-1项再相加，以阻碍编译器自动进行尾递归优化
// 注意如果n非常大会引发stack overflow，这里我们选择当n达到10^5的时候抛出异常
// 具体引发stack overflow的节点需要我们根据实际情况而定
class ArraySumReduceAndConquer : public ArraySum {
    int cal(span<const int> data) {
        if (auto size { data.size() }; size == 0) {
            return 0;
        } else if (size == 1) {
            return data[0];
        } else {
            return (*this)(data.first(size - 1)) + data[size - 1];
        }
    }
public:
    int operator()(span<const int> data) override {
        if (data.size() >= 10000) {
            throw runtime_error { "stack overflow" };
        }
        return cal(data);
    }
    string type_name() const override {
        return "ArraySum (Reduce & Conquer)";
    }
};

// 分治法求和，无论如何都很慢
// 破坏了数组访问的连续性，阻止编译器做SIMD优化，且不利于Cache优化
// 同时会浪费很多时间在栈上生成
class ArraySumDivideAndConquer : public ArraySum {
public:
    int operator()(span<const int> data) override {
        if (auto size { data.size() }; size == 0) {
            return 0;
        } else if (size == 1) {
            return data[0];
        } else {
            auto mid { size / 2 };
            return (*this)(data.first(mid)) + (*this)(data.last(size - mid));
        }
    }
    string type_name() const override {
        return "ArraySum (Divide & Conquer)";
    }
};

vector testSize { 1, 5, 100, 10000, 1'000'000, 100'000'000 };

// 可以将您的实现加入到测试框架的参数列表中
TestFramework<ArraySum, ArraySumBasic, ArraySumReduce, ArraySumIterative, ArraySumReduceAndConquer, ArraySumDivideAndConquer> test;

int main() {
    for (auto n : testSize) {
        cout << format("n = {}", n) << endl;
        vector<int> data(n);
        iota(begin(data), end(data), 1);
        test(data);
    }
}