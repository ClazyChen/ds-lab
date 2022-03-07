#pragma once

// 这是一个用于进行测试的小型框架
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <type_traits>
#include <tuple>
#include <typeinfo>
#include <iomanip>
using namespace std;

namespace clazy_framework {

class Algorithm {
public:
    virtual string getTypename() const {
        return typeid(*this).name();
    }
};

// BaseClass: 问题的基类（构造函数不需要参数）
// DerivedClass: 需要对比测试的子类
// 生成一组DerivedClass的实例，用于对比测试
template <typename BaseClass, typename... DerivedClass> 
	requires (is_base_of_v<BaseClass, DerivedClass> && ...)
auto generateInstances() {
    return vector<shared_ptr<BaseClass>> {
        (make_shared<DerivedClass>())...
    };
}

// 统计执行函数f需要的时间
double calculateTime(function<void()> f) {
    clock_t start, end;
    start = clock();
    f();
    end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

// 进行单一测试
template <typename BaseClass> requires (is_base_of_v<Algorithm, BaseClass>)
void applyTest(shared_ptr<BaseClass> instance, const function<void(shared_ptr<BaseClass>)>& test) {
    cout << "TEST [" << setw(28) << instance->getTypename() << "]\t";
    double seconds = calculateTime([&]() { test(instance); });
    cout << " (" << seconds << "s)" << endl;
}

// 进行对比测试
template <typename BaseClass> requires (is_base_of_v<Algorithm, BaseClass>)
void applyTest(vector<shared_ptr<BaseClass>>& instances, const function<void(shared_ptr<BaseClass>)>& test) {
    for (auto instance : instances) {
        applyTest(instance, test);
    }
}

}
