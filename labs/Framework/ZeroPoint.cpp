#include <vector>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
import Framework;
using namespace dslab;
using namespace std;

// �������
class ZeroPoint : public Algorithm<double, function<double(double)>> {
    static constexpr double limit_l { -1.0 };
    static constexpr double limit_r { 1.0 };
protected:
    static constexpr double eps { 1e-6 };
    virtual double apply(function<double(double)> f, double l, double r) = 0;
public:
    double operator()(function<double(double)> f) override {
        return apply(f, limit_l, limit_r);
    }
};

// ����Ķ��ֵ����㷨
class ZeroPointIterative : public ZeroPoint {
protected:
    double apply(function<double(double)> f, double l, double r) override {
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
};

// ������ת��Ϊ�ݹ�
class ZeroPointRecursive : public ZeroPoint {
protected:
    double apply(function<double(double)> f, double l, double r) override {
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
};

// �������������Եĺ������������������
class TestFunctor : public Algorithm<double, double> {
public:
    virtual string name() const = 0;
};

class TestFunctor1 : public TestFunctor {
public:
    double operator()(double x) override {
        return x * x - 2;
    }
    string name() const override {
        return "x^2 - 2";
    }
};

class TestFunctor2 : public TestFunctor {
public:
    double operator()(double x) override {
        return x - 0.5;
    }
    string name() const override {
        return "x - 1/2";
    }
};

class TestFunctor3 : public TestFunctor {
public:
    double operator()(double x) override {
        return exp(x) - 2;
    }
    string name() const override {
        return "e^x - 2";
    }
};

vector<shared_ptr<TestFunctor>> testFunctor {
    make_shared<TestFunctor1>(),
    make_shared<TestFunctor2>(),
    make_shared<TestFunctor3>()
};

// ���Խ�����ʵ�ּ��뵽���Կ�ܵĲ����б���
TestFramework<ZeroPoint, ZeroPointIterative, ZeroPointRecursive> test;

int main() {
    for (auto& f : testFunctor) {
        cout << format("Testing {}...", f->name()) << endl;
        test([&f](double x) { return f->operator()(x); });
    }
}