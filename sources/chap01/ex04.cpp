#include "test_framework.h"
using namespace clazy_framework;

// 这个例子展示了用递归方法分析迭代算法的技术
// 因为递归和递降本质上是一样的，所以递归的算法很适合用递降法进行分析
// 这个和我们做算法优化的方向是相反的
// 分析的时候：迭代到递归，方便用递降思想（书写时可以反过来，用数学归纳法）
// 优化的时候：递归到迭代，降低时间和空间（由于编译器会对递归算法做智能优化，不一定能真的降低）

// 函数零点问题
// 给定确定的误差限eps > 0
// 输入：函数f(x)，保证f(x)在数学上连续，且f(-1)*f(1) < 0
// 输出：函数f(x)在区间(-1,1)上的一个零点，绝对误差不超过eps

// 这里为了简单起见，只考虑(-1,1)这个区间
// 实际上也可以取成任意的区间

class ZeroPoint : public Algorithm<double, const function<double(double)>&> {
protected:
    constexpr static double eps = 1e-6;
    virtual double apply(const function<double(double)>& f, double l, double r) = 0;
public:
    double apply(const function<double(double)>& f) override {
        return apply(f, -1.0, 1.0);
    }
};

// 经典的循环版本的二分求解算法
// 高中数学必修3的内容
class ZeroPointIterative : public ZeroPoint {
protected:
    double apply(const function<double(double)>& f, double l, double r) override {
        while (r - l > eps) {             // 循环直到满足误差限
            double mid = l + (r - l) / 2; // 每次取(l,r)的中点
            if (f(l) * f(mid) <= 0) {     // 判断零点是否在(l,mid]中
                r = mid;                  
            } else {                      // 还是在(mid,r)中
                l = mid;
            }
        }
        return l;
    }
};

// 通常取中点的时候，使用 (l + r) / 2 或者 l + (r - l) / 2，都是可以的，属于推荐写法。
// 前者的优点是只需要进行一次加减法计算，而后者需要进行两次。
// 后者的优点在于可以保证不溢出。比如，当l和r都是32位有符号整数时，如果l = 1e9而r = 2e9，则前者会溢出，而后者不会。
// 还有一些写法，如 l / 2 + r / 2，当l和r是整型的时候不再适用（请您自己举出反例），属于不推荐的写法。

// 另外，请您自己思考：如果将f(l) * f(mid) <= 0这个判断条件的<=改为<，程序输出的结果会如何变化？
// 如果不容易得到一个思考的结果，可以再建立一个类进行这样的修改，通过实验来观察结果
// 作为一门工程学科，计算机学科非常需要您通过实验建立起来的经验。

// 递归版本的算法
// 它和迭代版本是完全对应的
// 迭代终止条件 直接对应于 递归边界
// 迭代的循环变量 直接对应于 递归参数
class ZeroPointRecursive : public ZeroPoint {
protected:
    double apply(const function<double(double)>& f, double l, double r) override {
        if (r - l < eps) {
            return l;                   // 递归边界
        }
        double mid = l + (r - l) / 2;   // 每次取(l,r)的中点
        if (f(l) * f(mid) <= 0) {       // 判断零点是否在(l,mid]中
            return apply(f, l, mid);
        } else {                        // 还是在(mid,r)中
            return apply(f, mid, r);
        }
    }
};

// 熟练地分析迭代和递归写法的关系很重要
// 比如，后面在二叉树遍历的时候，无论是邓俊辉《数据结构》还是王道上的写法
// 写的“递归版本”和“迭代版本”都不是真正对应的
// 如果没有意识到这一点，就很难深刻理解先序遍历和中序遍历的对称性
// 这一点将在后面的实验中具体体现

// 下面是用来测试的函数
// 您可以在这里添加更多的函数，用来测试您的算法
class TestFunction : public Algorithm<double, double> {

};

class TestFunction1 : public TestFunction {
public:
    double apply(double x) override {
        return x - 0.5;
    }
    string getTypeName() const override {
        return "x-1/2";
    }
};

class TestFunction2 : public TestFunction {
public:
    double apply(double x) override {
        return exp(x) - 2;
    }
    string getTypeName() const override {
        return "e^x-2";
    }
};

int main() {
    TestFramework<ZeroPoint, ZeroPointRecursive, ZeroPointIterative> tf;
    TestFramework<TestFunction, TestFunction1, TestFunction2> functions;
    for (auto& function : functions.instances) {
        cout << "Testing f(x) = " << function->getTypeName() << endl;
        tf.applyTest([&](auto algorithm) {
            cout << "answer = " << setw(11) << algorithm->apply(bind(&TestFunction::apply, function, placeholders::_1));
        });
    }
    return 0;
}
