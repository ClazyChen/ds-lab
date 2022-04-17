#pragma once

// 这个文件提供了一个测试用的框架
// 它用来生成对比测试的一组实例

#include "framework.h"

namespace clazy_framework {

// 统计执行函数f需要的时间
// 这里采用了chrono库，可以更好的控制时间，达到微秒级
// 当返回的值非常小时，可以认为是没有执行时间，不应该进行比较
// 因为这个时候可能会受到系统的影响

double calculateTime(function<void()> f) {
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count() / 1e6;
}

// BaseClass：问题的基类（构造函数不需要参数）
// DerivedClass：需要对比测试的子类
// 生成一组DerivedClass的实例，用于对比测试

template <typename BaseClass, typename... DerivedClass>
requires (is_base_of_v<Object, BaseClass> && sizeof...(DerivedClass) > 0 && (is_base_of_v<BaseClass, DerivedClass> && ...))
class TestFramework : public Object {
private:
    // 删除了拷贝构造函数、移动构造函数以及相应的赋值运算符
    TestFramework(const TestFramework<BaseClass, DerivedClass...>&) = delete;
    TestFramework(TestFramework<BaseClass, DerivedClass...>&&) = delete;
    TestFramework<BaseClass, DerivedClass...>& operator=(const TestFramework<BaseClass, DerivedClass...>&) = delete;
    TestFramework<BaseClass, DerivedClass...>& operator=(TestFramework<BaseClass, DerivedClass...>&&) = delete;

public:
    vector<shared_ptr<BaseClass>> instances;

private:
    // 默认的实例类型名输出宽度
    constexpr static int defaultWidth = 20;
    int instanceNameWidth() const {
        int width = defaultWidth;
        for (auto& instance : instances) {
            width = max(width, (int)instance->getTypeName().size());
        }
        return width;
    }

public:
    // 在构造函数中，初始化一组实例（至少需要含有1个实例）
    // 这里采用了不定参数的一个遍历形式
    // 通过生成一个伪数组，保证push_back行为是被正确执行的
    TestFramework() : instances{make_shared<DerivedClass>()...} {
        // int _[] {(instances.push_back(make_shared<DerivedClass>()), 0)... };
    }

    // // 对单个实例进行测试
    // void applyTest(shared_ptr<BaseClass> instance, const function<void(shared_ptr<BaseClass>)>& test, int width = defaultWidth) {
    //     cout << "TEST [" << setw(width) << instance->getTypeName() << "] \t"; // 输出实例类型名
    //     double seconds = calculateTime([&] { test(instance); }); // 计算测试所需时间
    //     cout << " (" << fixed << setprecision(6) << seconds << "s)" << endl; // 输出测试所需时间      
    // }

    // // 对所有实例进行测试
    // void applyTest(const function<void(shared_ptr<BaseClass>)>& test) {
    //     int width = defaultWidth;
    //     for (auto& instance : instances) {
    //         width = max<int>(width, instance->getTypeName().size());
    //     }
    //     for (auto& instance : instances) {
    //         applyTest(instance, test, width);
    //     }
    // }

    // 重置所有的实例
    void clear() {
        for (auto& instance : instances) {
            instance->clear();
        }
    }

    // 用给定的参数对所有实例进行测试
    // 这里存在两种情况：
    // 1. 各个测试实例运行过程中不会改动参数，如查找
    //    此时不需要制作参数的副本，可以直接调用
    // 2. 各个测试实例运行过程中会改动参数，如排序
    //    此时每次调用实例的时候，需要制作参数的副本
    // 这两种情况的区分，通过检查BaseClass的调用方法中，接受的是否是值（或const引用）还是普通引用
    // 从另一个维度，也可以分成两种情况
    // 1. 实例调用后没有返回值，此时只需要报告所用时间即可
    // 2. 实例调用后存在返回值，此时除了报告所用时间，还需要报告返回值
    template <typename... Args>
    void test(const Args&... args) {
        constexpr bool directInvoke = is_invocable_v<BaseClass, Args...>;
        constexpr bool referenceInvoke = !directInvoke && is_invocable_v<BaseClass, Args&...>;
        constexpr bool procedureInvoke = is_same_v<void, invoke_result_t<BaseClass, Args&...>>;
        static_assert(directInvoke || referenceInvoke, "BaseClass must have a valid invoke method");
        int width = instanceNameWidth();
        auto invoke = [&](auto& instance) {
            if constexpr (directInvoke) {
                return instance->apply(args...);
            } else /* referenceInvoke */ {
                return [&instance](Args... copied){ return instance->apply(copied...); }(args...);
            }
        };
        auto getResult = [&](auto& instance) {
            if constexpr (procedureInvoke) {
                invoke(instance);
                return 0;
            } else {
                return invoke(instance);
            }
        };
        for (auto& instance : instances) {
            cout << "TEST [" << setw(width) << instance->getTypeName() << "] \t"; // 输出实例类型名
            double seconds = calculateTime([&] {
                auto result = getResult(instance);
                if constexpr (!procedureInvoke) {
                    cout << "result = " << setw(11) << result << flush;
                }
            });
            cout << " (" << fixed << setprecision(6) << seconds << "s)" << endl; // 输出测试所需时间 
        }
    }

    // // 输出检验，用来判断各个实例的测试结果是否一致
    // // 检验方法是，首先计算第一个实例的结果
    // // 然后依次比较后续实例的结果是否和第一个实例一致
    // template <typename... Args>
    // void check(const function<void(shared_ptr<BaseClass>)>& test, const Args&... args) {
    //     constexpr bool directInvoke = is_invocable_v<BaseClass, Args...>;
    //     constexpr bool referenceInvoke = !directInvoke && is_invocable_v<BaseClass, Args&...>;
    //     static_assert(directInvoke || referenceInvoke, "BaseClass must have a valid invoke method");
    //     auto invoke = [&](auto& instance) {
    //         if constexpr (directInvoke) {
    //             return instance(args...);
    //         } else {
    //             return [&instance](Args... copied){ instance(copied...); }(args...);
    //         }
    //     };
    //     auto result = invoke(instances[0]);
    //     for (auto& instance : instances | views::drop(1)) {
    //         if (invoke(instance) != result) {
    //             cout << "CHECK FAILED" << endl;
    //             return;
    //         }
    //     }
    // }

};

}
