import Framework;
import std;

using namespace dslab;
using namespace std;

// 判断是否是2的幂次
class IsPower2 : public Algorithm<bool(int)> {};

// 直接判定的方法
class IsPower2Basic : public IsPower2 {
public:
    bool operator()(int n) override {
        return n > 0 && (n & (n - 1)) == 0;
    }
    string type_name() const override {
        return "IsPower2 (determinant)";
    }
};

// 利用std::has_single_bit
class IsPower2SingleBit : public IsPower2 {
public:
    bool operator()(int n) override {
        return n > 0 && has_single_bit(static_cast<unsigned>(n));
    }
    string type_name() const override {
        return "IsPower2 (std::has_single_bit)";
    }
};

// 递归的方法
class IsPower2Recursive : public IsPower2 {
public:
    bool operator()(int n) override {
        if (n % 2 == 1) {
            return n == 1;
        } else {
            return (*this)(n / 2);
        }
    }
    string type_name() const override {
        return "IsPower2 (Recursive)";
    }
};

// 迭代的方法
class IsPower2Iterative : public IsPower2 {
public:
    bool operator()(int n) override {
        int m { 1 };
        while (m < n) {
            m *= 2;
        }
        return m == n;
    }
    string type_name() const override {
        return "IsPower2 (Iterative)";
    }
};

vector testData {
    1, 2, 4, 6, 256, 65536,
    0x80000, 0x88888, 0x08'00'00'00 // 以上是几组常规数据
    // ,0x7f'ff'ff'ff // 较大的数据，但是会让上面的迭代版本陷入无限循环
    // ,0             // 无意义数据，但是会让上面的递归版本陷入无限递归
    // ,-1            // 无意义数据，但是会让上面的递归版本陷入无限递归
};

// 可以将您的实现加入到测试框架的参数列表中
TestFramework<IsPower2, IsPower2Basic, IsPower2SingleBit, IsPower2Recursive, IsPower2Iterative> test;

int main() {
    for (auto n : testData) {
        cout << format("n = {:>10} : ", n) << endl;
        test(n);
    }
    return 0;
}
