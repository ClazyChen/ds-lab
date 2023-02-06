module;
#include <vector>
#include <chrono>
#include <memory>
#include <type_traits>
#include <functional>
#include <format>
#include <iostream>

export module Framework.TestFramework;

import Framework.Object;
export namespace dslab {

template <typename Output>
std::pair<Output, double> reportTime(std::function<Output()> f) {
    auto start { std::chrono::high_resolution_clock::now() };
    decltype(auto) result { f() };
    auto end { std::chrono::high_resolution_clock::now() };
    auto time { std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1e6 };
    return { result, time };
}

double reportProcedureTime(std::function<void()> f) {
    auto start { std::chrono::high_resolution_clock::now() };
    f();
    auto end { std::chrono::high_resolution_clock::now() };
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1e6;
}

template <typename BaseClass, typename... DerivedClass>
    requires (std::is_base_of_v<BaseClass, DerivedClass> && ...) && std::is_base_of_v<Object, BaseClass>&&
    requires { typename BaseClass::Output; }
class TestFramework {
    using Output = typename BaseClass::Output;
    std::vector<std::shared_ptr<BaseClass>> m_instances;
    static constexpr size_t DEFAULT_WIDTH { 20 };
    size_t instanceNameWidth() const {
        size_t max { DEFAULT_WIDTH };
        for (const auto& instance : m_instances) {
            size_t length { instance->type_name().length() };
            if (length > max) {
                max = length;
            }
        }
        return max;
    }
public:
    template <typename... InputType>
        requires std::is_same_v<Output, void> || std::is_invocable_r_v<Output, BaseClass, InputType...>
    void printResult(InputType... inputs) {
        size_t width { instanceNameWidth() };
        for (const auto& instance : m_instances) {
            auto name { instance->type_name() };
            std::cout << std::format("TEST [{0:{1}}] ", name, width);
            try {
                if constexpr (std::is_same_v<Output, void> || std::is_reference_v<Output>) {
                    auto time { reportProcedureTime([&]() { (*instance)(std::forward<InputType>(inputs)...); }) };
                    std::cout << std::format("Time: {0:.6f} s", time) << std::endl;
                } else {
                    auto [result, time] { reportTime<Output>([&]() { return (*instance)(std::forward<InputType>(inputs)...); }) };
                    std::cout << std::format("Result: {0}, Time: {1:.6f} s", result, time) << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }

    template <typename... InputType>
        requires std::is_same_v<Output, void> || std::is_invocable_r_v<Output, BaseClass, InputType...>
    void operator()(InputType... inputs) {
        printResult(std::forward<InputType>(inputs)...);
    }

    void run(std::function<void(BaseClass&)> f) {
        for (const auto& instance : m_instances) {
            f(*instance);
        }
    }

private:
    void initialize() {
        (m_instances.push_back(std::make_shared<DerivedClass>()), ...);
    }

    BaseClass& operator[](size_t index) {
        return *m_instances[index];
    }

public:
    TestFramework() {
        initialize();
    }
};

}
