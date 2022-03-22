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
#include <ranges>
using namespace std;

namespace clazy_framework {

template <typename T>
class DataStructure {
public:
    // 您需要允许用户清空数据结构
    virtual void clear() = 0;
    // 判空函数，框架里会写
    virtual bool empty() const = 0;
};

template <typename T, typename Container>
concept is_data_structure = is_base_of_v<DataStructure<T>, Container>;

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

constexpr const static int defaultWidth = 28;

// 进行单一测试
template <typename BaseClass> requires (is_base_of_v<Algorithm, BaseClass>)
void applyTest(shared_ptr<BaseClass> instance, const function<void(shared_ptr<BaseClass>)>& test, int width = defaultWidth) {
    cout << "TEST [" << setw(width) << instance->getTypename() << "]\t";
    double seconds = calculateTime([&]() { test(instance); });
    cout << " (" << seconds << "s)" << endl;
}

// 进行对比测试
template <typename BaseClass> requires (is_base_of_v<Algorithm, BaseClass>)
void applyTest(vector<shared_ptr<BaseClass>>& instances, const function<void(shared_ptr<BaseClass>)>& test) {
    int width = defaultWidth;
    for (auto instance : instances) {
        width = max<int>(width, instance->getTypename().size() + 2);
    }
    for (auto instance : instances) {
        applyTest(instance, test, width);
    }
}

template <typename T>
using Comparator = function<bool(const T&, const T&)>;

}
