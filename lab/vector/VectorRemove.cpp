#include "vector.hpp"

using namespace dslab;

template <typename T>
class VectorRemove : public Algorithm<std::size_t(const T&)> {
protected:
    Vector<T> V {};
    virtual void remove(const T& e) = 0;
public:
    std::size_t operator()(const T& e) override {
        auto n { V.size() };
        remove(e);
        return n - V.size();
    }
    void initialize(std::size_t n) {
        V.resize(n);
        for (auto i { 0uz }; i < n; ++i) {
            V[i] = i % 2;
        }
    }
};

template <typename T>
class VectorRemoveBasic : public VectorRemove<T> {
    using VectorRemove<T>::V;
protected:
    void remove(const T& e) override {
        while (true) {
            if (auto r { std::find(V.begin(), V.end(), e) }; r != V.end()) {
                V.erase(r);
            } else {
                break;
            }
        }
    }
public:
    std::string type_name() const override {
        return "Separate Find, Separate Remove";
    }
};

template <typename T>
class VectorRemoveImproved : public VectorRemove<T> {
    using VectorRemove<T>::V;
protected:
    void remove(const T& e) override {
        auto r { V.begin() };
        while (r = std::find(r, V.end(), e), r != V.end()) {
            V.erase(r);
        }
    }
public:
    std::string type_name() const override {
        return "Global   Find, Separate Remove";
    }
};

template <typename T>
class VectorRemoveFSP : public VectorRemove<T> {
    using VectorRemove<T>::V;
protected:
    void remove(const T& e) override {
        auto fp { V.begin() }, sp { V.begin() };
        while (fp != V.end()) {
            if (*fp != e) {
                *sp++ = std::move(*fp);
            }
            ++fp;
        }
        V.resize(sp - V.begin());
    }
public:
    std::string type_name() const override {
        return "Global   Find, Batched  Remove (Fast & Slow Ptr)"; 
    }
};

template <typename T>
class VectorRemoveErase : public VectorRemove<T> {
    using VectorRemove<T>::V;
protected:
    void remove(const T& e) override {
        V.resize(std::remove(V.begin(), V.end(), e) - V.begin());
    }
public:
    std::string type_name() const override {
        return "Global   Find, Batched  Remove (Remove Erase Idiom)";
    }
};

std::vector testData { 10, 100, 1000, 10000, 100'000 };

TestFramework<VectorRemove<int>,
    VectorRemoveBasic<int>,
    VectorRemoveImproved<int>,
    VectorRemoveFSP<int>,
    VectorRemoveErase<int>> test;

int main() {
    for (auto n : testData) {
        std::cout << std::format("n = {}", n) << std::endl;
        test.run([n](auto& algo) { algo.initialize(n); });
        test(0);
    }
    return 0;
}
