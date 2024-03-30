#pragma once

#include "Algorithm.hpp"
#include "DataStructure.hpp"

namespace dslab::framework {

class TestItem {
public:
    std::size_t m_value { 0 };
    static std::size_t s_copyCount;
    static std::size_t s_moveCount;
    static void reset() {
        s_copyCount = 0;
        s_moveCount = 0;
    }
    TestItem() = default;
    TestItem(std::size_t value) : m_value(value) {}
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

std::size_t TestItem::s_copyCount { 0 };
std::size_t TestItem::s_moveCount { 0 };

template <template <typename> typename S, typename T, typename... F>
    requires std::is_base_of_v<DataStructure<T>, S<T>> && (std::is_base_of_v<Algorithm<void(S<T>&)>, F> && ...)
class StructureTestFramework : public Algorithm<void(S<T>&)> {
    std::size_t m_stepCount { 0 };
    std::vector<std::unique_ptr<Algorithm<void(S<T>&)>>> m_functors;

    void step(S<T>& structure, Algorithm<void(S<T>&)>& functor) {
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
        (m_functors.push_back(std::make_unique<F>()), ...);
    }

    void operator()(S<T>& structure) override {
        m_stepCount = 0;
        for (auto&& functor : m_functors) {
            step(structure, *functor);
        }
    }
};

}

template <>
struct std::formatter<dslab::framework::TestItem> : std::formatter<std::string> {
    template <typename FormatContext>
    auto format(const dslab::framework::TestItem& item, FormatContext& ctx) const {
        return std::formatter<std::string>::format(std::format("{}", item.m_value), ctx);
    }
};