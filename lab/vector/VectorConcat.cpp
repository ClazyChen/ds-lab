#include "vector.hpp"

using namespace dslab;

template <typename T>
class VectorConcat : public Algorithm<Vector<T>&(std::size_t)> {
protected:
    Vector<T> V, W;
public:
    void initialize(std::size_t n, std::size_t m) {
        V.reserve(n + m);
        W.reserve(m);
        V.resize(n);
        W.resize(m);
    }
};

template <typename T>
class VectorConcatBasic : public VectorConcat<T> {
    using VectorConcat<T>::V, VectorConcat<T>::W;
public:
    Vector<T>& operator()(std::size_t r) override {
        auto pos { V.begin() + r };
        for (auto&& e : W) {
            V.insert(pos++, std::move(e));
        }
        return V;
    }
    std::string type_name() const override {
        return "Concat (Insert one by one)";
    }
};

template <typename T>
class VectorConcatFast : public VectorConcat<T> {
    using VectorConcat<T>::V, VectorConcat<T>::W;
public:
    Vector<T>& operator()(std::size_t r) override {
        V.resize(V.size() + W.size());
        std::move_backward(V.begin() + r, V.end() - W.size(), V.end());
        std::move(W.begin(), W.end(), V.begin() + r);
        return V;
    }
    std::string type_name() const override {
        return "Concat (Overall Move)";
    }
};

std::vector<std::tuple<std::size_t, std::size_t, std::size_t>> testData {
    { 20'0000, 100, 0 }, // insert a few elements at the beginning
    { 20'0000, 100'000, 0 }, // insert a lot of elements at the beginning
    { 20'0000, 100, 20'0000 }, // insert a few elements at the end
    { 20'0000, 100'000, 20'0000 } // insert a lot of elements at the end
};

TestFramework<VectorConcat<int>, VectorConcatBasic<int>, VectorConcatFast<int>> test;

int main() {
    for (auto [n, m, r] : testData) {
        std::cout << std::format("n = {:>7}, m = {:>7}, r = {:>7}", n, m, r) << std::endl;
        test.run([n, m](auto& algo) { algo.initialize(n, m); });
        test(r);
    }
}