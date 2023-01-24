#include <vector>
#include <format>
#include <iostream>
import Framework;
import Vector;
using namespace dslab;
using namespace std;

class VectorInsertProblem : public Algorithm<int, int> {
public:
    virtual void reset() = 0;
};

template <typename Vec>
    requires is_base_of_v<AbstractVector<int>, Vec>
class VectorInsertBasic : public VectorInsertProblem {
    Vec V;
public:
    int operator()(int n) override {
        for (size_t i { 0 }; i < n; ++i) {
            V.insert(V.size(), i);
        }
        return V.capacity();
    }
    void reset() override {
        V.clear();
    }
};

template <typename Alloc>
    requires is_base_of_v<AbstractVectorAllocator, Alloc>
class VectorInsert : public VectorInsertBasic<Vector<int, Alloc>> {
public:
    string type_name() const override {
        return Alloc {}.type_name();
    }
};

vector testData { 100, 1000, 10000, 100'000, 1'000'000 };

TestFramework<VectorInsertProblem,
    VectorInsert<VectorAllocatorAP<64>>,
    VectorInsert<VectorAllocatorAP<4096>>,
    VectorInsert<VectorAllocatorGP<ratio<3, 2>>>,
    VectorInsert<VectorAllocatorGP<ratio<2>>>,
    VectorInsert<VectorAllocatorGP<ratio<4>>>> test;

int main() {
    for (auto n : testData) {
        cout << format("n = {}", n) << endl;
        test.run(&VectorInsertProblem::reset);
        test(n);
    }
    return 0;
}