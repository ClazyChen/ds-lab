#pragma once

// 这个文件提供了所有后续代码的基本框架
// 包括以下内容：
// 1. 常用的标准库的包含声明
//    我直接包含了std命名空间，对于写库的人员来说，这是一个不正确的选择
//    但为了简便起见，基于大多数普通C++使用者的编程习惯，我还是包含了std命名空间
// 2. 数据结构和算法的基类定义
//    我们的代码中，所有的数据结构和算法都是继承自这个基类
// 3. 一些基本的概念和别名定义

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

// 最基本的基类，用于测试
// 这里只实现了一个获取类型名的接口
// 继承于它的基类，都可以通过调用这个接口，以反射的方式获取类型名
// C++20为止，仍然没有很好地支持反射功能，所以这里暂时通过设计这个基类的方式
// 给用户获取一个数据结构的名字或算法的可能
// 这里得到的名字可能会在可读性上有一些缺陷
// 比如，由于示例代码大量用到了模板，所以可能生成的名字会过于冗长
// 解决这个问题的方法，是在final类中重载这个函数，返回一个更加可读的名字

class Object {
public:
    virtual string getTypeName() const {
        return typeid(*this).name();
    }
};

// 所有的数据结构都基于这个DataStructure抽象基类
// 对于一个基本的数据结构，它只有三个抽象接口
// 1. 判空
// 2. 清空
// 3. 获取数据结构的规模（即存放在数据结构中的元素个数）
//    尽管更加可读的做法是返回size_t类型，但为了避免signed/unsigned引起的不必要麻烦，这里让规模返回int

template <typename T>
class DataStructure : public Object {
public:
    virtual bool empty() const = 0;
    virtual void clear() = 0;
    virtual int size() const = 0; 
};

// 所有的算法都基于这个Algorithm抽象基类
// 对于一个基本的算法，它只有一个抽象接口
// 它接收的不定参数列表Args，用来表示算法的输入数据
// 返回值类型模板参数Result，用来表示算法的输出数据
// 用户需要重载apply这个接口，以实现算法的具体实现
// 我重载了函数运算符（括号），用来调用apply，使得Algorithm的派生类可以生成仿函数（functor）对象
// 仿函数可以被赋值给std::function

template <typename Result = void, typename... Args>
class Algorithm : public Object {
public:
    virtual Result apply(Args... args) = 0;
    Result operator()(Args... args) {
        return apply(args...);
    }
};

// 判断是否是数据结构的概念

template <typename T, typename Container>
concept is_data_structure = is_base_of_v<DataStructure<T>, Container>;

// 比较器的别名，用来在查找和排序中使用

template <typename T>
using Comparator = function<bool(const T&, const T&)>;

// 当表示索引的时候，用Rank代替int
// 和邓俊辉《数据结构》保持一致

using Rank = int;

}
