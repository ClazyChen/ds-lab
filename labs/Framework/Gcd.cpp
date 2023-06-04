import Framework;
import Gcd;
import std;

using namespace dslab;
using namespace std;

// 最大公约数问题
// 基类参见Gcd.ixx

// 暴力枚举法
class GcdBasic : public GcdProblem {
public:
    int operator()(int a, int b) override {
        int gcd { 0 };
        for (int i { 1 }; i <= a && i <= b; ++i) {
            if (a % i == 0 && b % i == 0) {
                gcd = i;
            }
        }
        return gcd;
    }
    string type_name() const override {
        return "Gcd (Naive)";
    }
};

// 递归的欧几里得算法
// 在编译之后，往往会被优化成迭代的形式
// 但在数据结构中，我们保留递归形式进行分析
class GcdEuclid : public GcdProblem {
public:
    int operator()(int a, int b) override {
        if (b == 0) {
            return a;
        } else {
            return (*this)(b, a % b);
        }
    }
    string type_name() const override {
        return "Euclidean Gcd (Recursive)";
    }
};

// 在Gcd.ixx中有一个迭代实现
using GcdEuclidIterative = Gcd;

vector<pair<int, int>> testData {
    {1, 1}, {1, 6},
    { 100'000'008, 12'345'678 },
    {24, 18}, {720, 1024},
    {0, 0},   // 未定义行为
    {0, 6},   // 未定义行为
    {-8, 6},  // 未定义行为
    {6, -8},  // 未定义行为
    {-8, -8}, // 未定义行为
    {-8, 4},  // 未定义行为
    {8, -6}   // 未定义行为
};

// 当我们设计算法的时候，假定输入数据a和b都是正整数
// 所以当我们输入负数或者0时，实际上是未定义行为
// 此时返回的结果是无法控制的，比如一正一负的情况下，结果的符号是不确定的
// 如果您追求更好的稳健性，可以为非正数的情况下补充合理的定义

// 可以将您的实现加入到测试框架的参数列表中
TestFramework<GcdProblem, GcdBasic, GcdEuclid, GcdEuclidIterative> test;

int main() {
    for (auto& [a, b] : testData) {
        cout << format("Gcd({:>10}, {:>10})", a, b) << endl;
        test(a, b);
    }
}