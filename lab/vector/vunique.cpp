#include "vector.hpp"

using namespace dslab;

template <typename T>
using VectorUnique = Algorithm<void(Vector<T>&)>;

template <typename T>
class VectorUniqueBasic : public VectorUnique<T> {
public:
    void operator()(Vector<T>& V) override {
        for (auto i { V.begin() }; i != V.end(); ++i) {
            V.resize(std::remove(i + 1, V.end(), *i) - V.begin());
        }
    }
    std::string type_name() const override {
        return "Unique one by one (Remove Erase Idiom)";
    }
};

template <typename T>
class VectorUniqueBasic2 : public VectorUnique<T> {
public:
    void operator()(Vector<T>& V) override {
        for (auto i { V.begin() }; i != V.end(); ++i) {
            auto j { i };
            while (j = std::find(i + 1, V.end(), *i), j != V.end()) {
                V.erase(j);
            }
        }
    }
    std::string type_name() const override {
        return "Unique one by one (find and remove)";
    }
};

template <typename T>
class VectorUniqueFSP : public VectorUnique<T> {
public:
    void operator()(Vector<T>& V) override {
        auto sp { V.begin() }, fp { V.begin() };
        while (++fp != V.end()) {
            if (*sp != *fp) {
                *++sp = std::move(*fp);
            }
        }
        V.resize(++sp - V.begin());
    }
    std::string type_name() const override {
        return "Unique globally   (Fast & Slow Ptr / Sorted only)";
    }
};

template <typename T>
class VectorUniqueSort : public VectorUnique<T> {
    struct Item {
        T value;
        std::size_t rank;
        bool operator==(const Item& rhs) const {
            return value == rhs.value;
        }
        auto operator<=>(const Item& rhs) const {
            return value <=> rhs.value;
        }
    };
    Vector<Item> W;
    void moveToW(Vector<T>& V) {
        W.clear();
        std::transform(V.begin(), V.end(), std::back_inserter(W), [](const T& e) {
            return Item { e, 0 };
        });
    }
    void moveFromW(Vector<T>& V) {
        V.clear();
        std::transform(W.begin(), W.end(), std::back_inserter(V), [](const Item& item) {
            return item.value;
        });
    }
public:
    void operator()(Vector<T>& V) override {
        moveToW(V);
        std::sort(W.begin(), W.end());
        W.resize(std::unique(W.begin(), W.end()) - W.begin());
        std::sort(W.begin(), W.end(), [](const Item& lhs, const Item& rhs) {
            return lhs.rank < rhs.rank;
        });
        moveFromW(V);
    }
    std::string type_name() const override {
        return "Unique globally   (Sort First)";
    }
};

template <typename T>
class VectorUniqueTest : public Algorithm<std::size_t()> {
protected:
    Vector<T> V;
public:
    void initialize(const Vector<int>& V) {
        this->V = V;
    }
};

template <typename T, template<typename> typename Unique>
    requires std::is_base_of_v<VectorUnique<T>, Unique<T>>
class VectorUniqueTestImpl : public VectorUniqueTest<T> {
    Unique<T> unique;
    using VectorUniqueTest<T>::V;
public:
    std::size_t operator()() override {
        unique(V);
        return V.size();
    }
    std::string type_name() const override {
        return unique.type_name();
    }
};

std::vector testData { 10, 100, 1000, 10000 };

TestFramework<VectorUniqueTest<int>,
    VectorUniqueTestImpl<int, VectorUniqueBasic>,
    VectorUniqueTestImpl<int, VectorUniqueBasic2>,
    VectorUniqueTestImpl<int, VectorUniqueSort>> test;

void testCase(std::function<Vector<int>(std::size_t)> generator) {
    for (auto n : testData) {
        std::cout << std::format("n = {:6d} ", n) << std::endl;
        auto V { generator(n) };
        test.run([&V](auto& problem) { problem.initialize(V); });
        test();
    }
}

int main() {
    std::cout << "worst case scenario" << std::endl;
    testCase([](std::size_t n) {
        Vector<int> V(n);
        std::iota(V.begin(), V.end(), 0);
        return V;
    });
    std::cout << "best case scenario" << std::endl;
    testCase([](std::size_t n) { return Vector<int>(n); });
    return 0;
}