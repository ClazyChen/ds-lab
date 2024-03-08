#include "framework.hpp"
#include <cmath>

using namespace dslab;

class ZeroPoint : public Algorithm<double(std::function<double(double)>)> {
    static constexpr double limit_l { -1.0 };
    static constexpr double limit_r { 1.0 };
protected:
    static constexpr double eps { 1e-6 };
    using funcdd = std::function<double(double)>;
    virtual double apply(funcdd f, double l, double r) = 0;
public:
    double operator()(funcdd f) override {
        return apply(f, limit_l, limit_r);
    }
};

// classic bisection iterative algorithm
class ZeroPointIterative : public ZeroPoint {
protected:
    double apply(funcdd f, double l, double r) override {
        while (r - l > eps) {
            double mid { l + (r - l) / 2 };
            if (f(l) * f(mid) <= 0) {
                r = mid;
            } else {
                l = mid;
            }
        }
        return l;
    }
    std::string type_name() const override {
        return "Bisection (Iterative)";
    }
};

// convert iterative to recursive
class ZeroPointRecursive : public ZeroPoint {
protected:
    double apply(funcdd f, double l, double r) override {
        if (r - l <= eps) {
            return l;
        } else {
            double mid { l + (r - l) / 2 };
            if (f(l) * f(mid) <= 0) {
                return apply(f, l, mid);
            } else {
                return apply(f, mid, r);
            }
        }
    }
    std::string type_name() const override {
        return "Bisection (Recursive)";
    }
};

// test functions
using TestFunctor = Algorithm<double(double)>;
class TestFunctor1 : public TestFunctor {
public:
    double operator()(double x) override {
        return x * x * x - 0.125;
    }
    std::string type_name() const override {
        return "x^3 - 1/8";
    }
};

class TestFunctor2 : public TestFunctor {
public:
    double operator()(double x) override {
        return x - 0.5;
    }
    std::string type_name() const override {
        return "x - 1/2";
    }
};

class TestFunctor3 : public TestFunctor {
public:
    double operator()(double x) override {
        return std::exp(x) - 2;
    }
    std::string type_name() const override {
        return "exp(x) - 2";
    }
};

std::vector<std::shared_ptr<TestFunctor>> testFunctors {
    std::make_shared<TestFunctor1>(),
    std::make_shared<TestFunctor2>(),
    std::make_shared<TestFunctor3>()
};

TestFramework<ZeroPoint, ZeroPointIterative, ZeroPointRecursive> test;

int main() {
    for (auto& f : testFunctors) {
        std::cout << std::format("Testing {:>8} ...", f->type_name()) << std::endl;
        test([&f](double x) { return (*f)(x); });
    }
}