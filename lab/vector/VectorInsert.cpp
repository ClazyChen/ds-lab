#include "vector.hpp"

using namespace dslab;

class VectorInsertProblem : public Algorithm<std::size_t(std::size_t)> {
public:
    virtual void reset() = 0;
};

template <typename V>
    requires std::is_base_of_v<AbstractVector<std::size_t>, V>
class VectorInsert : public VectorInsertProblem {
    V v;
public:
    std::size_t operator()(std::size_t n) override {
        for (auto i { 0uz }; i < n; ++i) {
            v.insert(v.end(), i);
        }
        return v.capacity();
    }
    void reset() override {
        v.clear();
    }
};

template <typename A>
    requires std::is_base_of_v<VectorAllocator, A>
class VectorInsertImpl : public VectorInsert<Vector<std::size_t, A>> {
public:
    std::string type_name() const override {
        return std::format("Expand with {}", A {}.type_name());
    }
};

std::vector testData { 20'0000, 40'0000, 60'0000, 80'0000, 100'0000 };

TestFramework<VectorInsertProblem,
    VectorInsertImpl<VectorAllocatorAP<64>>,
    VectorInsertImpl<VectorAllocatorAP<4096>>,
    VectorInsertImpl<VectorAllocatorGP<std::ratio<3, 2>>>,
    VectorInsertImpl<VectorAllocatorGP<std::ratio<2>>>,
    VectorInsertImpl<VectorAllocatorGP<std::ratio<4>>>> test;

int main() {
    for (auto n : testData) {
        std::cout << std::format("n = {}", n) << std::endl;
        test.run(&VectorInsertProblem::reset);
        test(n);
    }
    return 0;
}