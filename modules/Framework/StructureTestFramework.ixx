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