#pragma once

// 这是一个用于进行测试的小型框架
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <type_traits>
#include <tuple>
#include <typeinfo>
#include <iomanip>
#include <ranges>
#include <cassert>
using namespace std;

namespace clazy_framework {

template <typename T>
class DataStructure {
public:
    // 您需要允许用户清空数据结构
    virtual void clear() = 0;
    // 判空函数，框架里会写
    virtual bool empty() const = 0;
    // 获取类型名
    virtual string getTypename() const {
        return typeid(*this).name();
    }
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
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count() / 1e6;
}

constexpr const static int defaultWidth = 28;

// 进行单一测试
template <typename BaseClass> requires (is_base_of_v<Algorithm, BaseClass>)
void applyTest(shared_ptr<BaseClass> instance, const function<void(shared_ptr<BaseClass>)>& test, int width = defaultWidth) {
    cout << "TEST [" << setw(width) << instance->getTypename() << "]\t";
    double seconds = calculateTime([&]() { test(instance); });
    cout << " (" << fixed << setprecision(6) << seconds << "s)" << endl;
}

// 进行对比测试
template <typename BaseClass> requires (is_base_of_v<Algorithm, BaseClass>)
void applyTest(vector<shared_ptr<BaseClass>>& instances, const function<void(shared_ptr<BaseClass>)>& test) {
    int width = defaultWidth;
    for (auto instance : instances) {
        width = max<int>(width, instance->getTypename().size());
    }
    for (auto instance : instances) {
        applyTest(instance, test, width);
    }
}

// 比较器
template <typename T>
using Comparator = function<bool(const T&, const T&)>;

using Rank = int; // 索引的时候使用Rank代替int

}
