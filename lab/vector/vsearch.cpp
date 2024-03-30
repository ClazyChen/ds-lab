#include "search.hpp"
#include "vector.hpp"

using namespace dslab;

template <typename T>
class BinarySearchRecursive : public AbstractSearch<T> {
    using iterator = typename Vector<T>::iterator;
    using AbstractSearch<T>::cmp;
    iterator search(const Vector<T>& V, const T& e, iterator lo, iterator hi) const {
        if (std::distance(lo, hi) <= 1) {
            if (lo != V.end() && cmp(*lo, e)) {
                return hi;
            } else {
                return lo;
            }
        }
        auto mi { lo + std::distance(lo, hi) / 2 };
        if (cmp(e, *mi)) {
            return search(V, e, lo, mi);
        } else {
            return search(V, e, mi, hi);
        }
    }
protected:
    iterator search(const Vector<T>& V, const T& e) override {
        return search(V, e, V.begin(), V.end());
    }

public:
    std::string type_name() const override {
        return "Binary Search (Recursive)";
    }
};

template <typename T>
class BinarySearchIterative : public AbstractSearch<T> {
    using iterator = typename Vector<T>::iterator;
    using AbstractSearch<T>::cmp;
protected:
    iterator search(const Vector<T>& V, const T& e) override {
        auto lo { V.begin() }, hi { V.end() };
        while (std::distance(lo, hi) > 1) {
            auto mi { lo + std::distance(lo, hi) / 2 };
            if (cmp(e, *mi)) {
                hi = mi;
            } else {
                lo = mi;
            }
        }
        if (lo != V.end() && cmp(*lo, e)) {
            return hi;
        } else {
            return lo;
        }
    }
public:
    std::string type_name() const override {
        return "Binary Search (Iterative)";
    }
};

template <typename T, typename... Args>
class TailRecursive {
protected:
    virtual bool is_base_case(Args... args) = 0;
    virtual T base_case(Args... args) = 0;
    virtual void next_args(Args&... args) = 0;
public:
    T recursive(Args... args) {
        if (is_base_case(args...)) {
            return base_case(args...);
        } else {
            next_args(args...);
            return recursive(args...);
        }
    }
    T iterative(Args... args) {
        while (!is_base_case(args...)) {
            next_args(args...);
        }
        return base_case(args...);
    }
};

template <typename T>
class TailRecursiveImpl : public TailRecursive<
    typename Vector<T>::iterator,
    typename Vector<T>::iterator,
    typename Vector<T>::iterator
> {
protected:
    const Vector<T>& V;
    const T& e;
    const std::function<bool(const T&, const T&)>& cmp;
    using iterator = typename Vector<T>::iterator;
    bool is_base_case(iterator lo, iterator hi) override {
        return std::distance(lo, hi) <= 1;
    }
    iterator base_case(iterator lo, iterator hi) override {
        if (lo != V.end() && cmp(*lo, e)) {
            return hi;
        } else {
            return lo;
        }
    }
    void next_args(iterator& lo, iterator& hi) override {
        auto mi { lo + std::distance(lo, hi) / 2 };
        if (cmp(e, *mi)) {
            hi = mi;
        } else {
            lo = mi;
        }
    }
public:
    TailRecursiveImpl(const Vector<T>& V, const T& e, const std::function<bool(const T&, const T&)>& cmp = std::less<T>()) : V(V), e(e), cmp(cmp) {}
};

template <typename T>
class BinarySearchTailRecursive : public AbstractSearch<T> {
    std::unique_ptr<TailRecursiveImpl<T>> m_impl;
    using iterator = typename Vector<T>::iterator;
    using AbstractSearch<T>::cmp;
protected:
    iterator search(const Vector<T>& V, const T& e) override {
        m_impl = std::make_unique<TailRecursiveImpl<T>>(V, e, cmp);
        return m_impl->recursive(V.begin(), V.end());    
    }
public:
    std::string type_name() const override {
        return "Binary Search (Recursive with template)";
    }
};

template <typename T>
class BinarySearchTailRecursiveIterator : public AbstractSearch<T> {
    std::unique_ptr<TailRecursiveImpl<T>> m_impl;
    using iterator = typename Vector<T>::iterator;
    using AbstractSearch<T>::cmp;
protected:
    iterator search(const Vector<T>& V, const T& e) override {
        m_impl = std::make_unique<TailRecursiveImpl<T>>(V, e, cmp);
        return m_impl->iterative(V.begin(), V.end());    
    }
public:
    std::string type_name() const override {
        return "Binary Search (Iterative with template)";
    }
};

template <typename T>
class BinarySearchTest : public Algorithm<std::size_t(const Vector<T>&, const T&)> {
public:
    void initialize(Vector<T>& V, std::size_t n, std::function<T()> random) {
        V.resize(n);
        std::generate(V.begin(), V.end(), random);
        std::sort(V.begin(), V.end());
    }
};

template <typename T, template<typename> typename Search>
    requires std::is_base_of_v<AbstractSearch<T>, Search<T>>
class BinarySearchTestImpl : public BinarySearchTest<T> {
    Search<T> m_search;
    static constexpr std::size_t TIMES { 10000000 };
public:
    std::size_t operator()(const Vector<T>& V, const T& e) override {
        auto r { m_search(V, e) };
        for (auto i { 0uz }; i < TIMES; ++i) {
            if (r != m_search(V, e)) {
                throw std::runtime_error("BinarySearchTest: " + m_search.type_name() + " failed");
            }
        }
        return r - V.begin();
    }
    std::string type_name() const override {
        return m_search.type_name();
    }
};

std::vector testData { 10, 100, 1000, 10000, 100'000, 1'000'000 };

constexpr std::size_t random() {
    constexpr std::size_t lo { 0 }, hi { 5 };
    return Random::get(lo, hi);
}

TestFramework<BinarySearchTest<std::size_t>,
    BinarySearchTestImpl<std::size_t, BinarySearchRecursive>,
    BinarySearchTestImpl<std::size_t, BinarySearchIterative>,
    BinarySearchTestImpl<std::size_t, BinarySearchTailRecursive>,
    BinarySearchTestImpl<std::size_t, BinarySearchTailRecursiveIterator>
> test;

int main() {
    for (Vector<std::size_t> V; auto n : testData) {
        std::cout << std::format("n = {}", n) << std::endl;
        test.run([&V, n](BinarySearchTest<std::size_t>& test) {
            test.initialize(std::ref(V), n, random);
        });
        test(std::cref(V), random());
    }
}