#include "vector.hpp"
#include "list.hpp"
#include "forward_list.hpp"

using namespace dslab;

class ContinuousPush : public Algorithm<void(std::size_t)> {
public:
    virtual void initialize() = 0;
};

template <template<typename> typename L>
requires std::is_base_of_v<LinearList<std::size_t, typename L<std::size_t>::iterator, typename L<std::size_t>::const_iterator>, L<std::size_t>>
class ContinuousPushBack : public ContinuousPush {
    L<std::size_t> m_container;
public:
    void initialize() override {
        m_container.clear();
    }
    void operator()(std::size_t n) override {
        for (auto i { 0uz }; i < n; ++i) {
            m_container.push_back(i);
        }
    }
    std::string type_name() const override {
        return m_container.type_name();
    }
};

template <template<typename> typename L>
requires std::is_base_of_v<LinearList<std::size_t, typename L<std::size_t>::iterator, typename L<std::size_t>::const_iterator>, L<std::size_t>>
class ContinuousPushFront : public ContinuousPush {
    L<std::size_t> m_container;
public:
    void initialize() override {
        m_container.clear();
    }
    void operator()(std::size_t n) override {
        if constexpr (std::is_base_of_v<Vector<std::size_t>, L<std::size_t>>) {
            if (n >= 1000'000) {
                throw std::runtime_error { R"(Vector::push_front() is too slow for n >= 100'000)" };
            }
        }
        for (auto i { 0uz }; i < n; ++i) {
            m_container.push_front(i);
        }
    }
    std::string type_name() const override {
        return m_container.type_name();
    }
};

std::vector<std::size_t> testData { 10, 1000, 10'000, 100'000, 1'000'000, 3'000'000 };

TestFramework<ContinuousPush, ContinuousPushBack<Vector>, ContinuousPushBack<List>, ContinuousPushBack<ForwardList>> test_back;
TestFramework<ContinuousPush, ContinuousPushFront<Vector>, ContinuousPushFront<List>, ContinuousPushFront<ForwardList>> test_front;

template <typename T>
void runTest(T& test) {
    for (auto n : testData) {
        std::cout << std::format("n = {:6d} ", n) << std::endl;
        test.run([](auto& algo) { algo.initialize(); });
        test(n);
    }
}

int main() {
    std::cout << "Push Back Test" << std::endl;
    runTest(test_back);
    std::cout << "Push Front Test" << std::endl;
    runTest(test_front);
    return 0;
}