module;
#include <iostream>
#include <vector>
#include <memory>
#include <format>
#include <type_traits>

export module Framework.StructureTestFramework;

import Framework.Algorithm;
import Framework.DataStructure;

export namespace dslab {

// 用于进行测试的元素类型
class TestItem {
public:
    size_t m_value { 0 };
    static size_t s_copyCount;
    static size_t s_moveCount;
    TestItem() = default;
    TestItem(size_t value) : m_value(value) {}
    TestItem(const TestItem& other) : m_value(other.m_value) {
        ++s_copyCount;
    }
    TestItem(TestItem&& other) : m_value(other.m_value) {
        ++s_moveCount;
    }
    TestItem& operator=(const TestItem& other) {
        m_value = other.m_value;
        ++s_copyCount;
        return *this;
    }
    TestItem& operator=(TestItem&& other) {
        m_value = other.m_value;
        ++s_moveCount;
        return *this;
    }
    bool operator==(const TestItem& other) const {
        return m_value == other.m_value;
    }
    auto operator<=>(const TestItem& other) const = default;
};

size_t TestItem::s_copyCount { 0 };
size_t TestItem::s_moveCount { 0 };

std::ostream& operator<<(std::ostream& os, const TestItem& item) {
    return os << std::format("{}", item.m_value);
}

template <template <typename> typename Structure, typename T, typename... Functors>
    requires std::is_base_of_v<DataStructure<T>, Structure<T>> && (std::is_base_of_v<Algorithm<void(Structure<T>&)>, Functors> && ...)
class StructureTestFramework : public Algorithm<void(Structure<T>&)> {
    size_t m_stepCount { 0 };
    std::vector<std::unique_ptr<Algorithm<void(Structure<T>&)>>> m_functors;
    
    void step(Structure<T>& structure, Algorithm<void(Structure<T>&)>& functor) {
        std::cout << std::format("Step {:3}: {}", ++m_stepCount, functor.type_name()) << std::endl;
        try {
            functor(structure);
            std::cout << "\tPassed!" << std::endl;
        } catch (const std::exception& e) {
            std::cout << std::format("\tError: {}", e.what()) << std::endl;
        }
        std::cout << std::endl;
    }

public:
    void initialize() {
        (m_functors.push_back(std::make_unique<Functors>()), ...);
    }
    
    void operator()(Structure<T>& structure) override {
        m_stepCount = 0;
        for (auto&& functor : m_functors) {
            step(structure, *functor);
        }
    }
};


}