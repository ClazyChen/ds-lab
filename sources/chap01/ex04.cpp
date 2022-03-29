#include <cmath>
#include "framework.h"
using namespace clazy_framework;
// 这个例子展示了用递归方法分析迭代算法的技术
// 因为递归和递降本质上是一样的，所以递归的算法很适合用递降法进行分析
// 这个和我们做算法优化的方向是相反的
// 分析的时候：迭代到递归，方便用化归思想
// 优化的时候：递归到迭代，降低时间（常数）和空间

// 函数零点问题
// 输入：函数f(x)，区间(l,r)，误差限eps > 0
    /* 输入数据保证f(x)在数学上连续，且f(l)*f(r) < 0 */
// 输出：函数f(x)在区间(l,r)上的一个零点，绝对误差不超过eps

class ZeroPoint : public Algorithm {
protected:
    const double eps = 1e-6;
public:
    virtual double solve(const function<double(double)>& f, double l, double r) const = 0;
};

// 经典的循环版本的二分求解算法
// 高中数学必修3的内容
class ZeroPointIterative : public ZeroPoint {
public:
    double solve(const function<double(double)>& f, double l, double r) const override {
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
public:
    double solve(const function<double(double)>& f, double l, double r) const override {
        if (r - l < eps) {
            return l;                   // 递归边界
        }
        double mid = l + (r - l) / 2;   // 每次取(l,r)的中点
        if (f(l) * f(mid) <= 0) {       // 判断零点是否在(l,mid]中
            return solve(f, l, mid);
        } else {                        // 还是在(mid,r)中
            return solve(f, mid, r);
        }
    }
};

int main() {
    auto algorithms = generateInstances<ZeroPoint, ZeroPointIterative, ZeroPointRecursive>();
    auto test = [&](const function<double(double)>& f, string name, double l, double r) {
        cout << "Testing f(x) = " << name << endl; 
        applyTest<ZeroPoint>(algorithms, [&f, l, r](auto zp) {
            cout << "answer = " << setw(11) << zp->solve(f, l, r);
        });
    };

    // 下面展示三个例子
    test([](double x) { return exp(x) - 2; }, "e^x-2", 0,   1);
    test([](double x) { return x      - 1; }, "x-1",   0,   2); // 结果可能并不准确，但满足误差限要求
    test([](double x) { return x      - 1; }, "x-1",   0.9, 2); // 此外，结果和初始给定的区间(l,r)相关
    return 0;
}
