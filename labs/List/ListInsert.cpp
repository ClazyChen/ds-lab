import Framework;
import LinearList;
import Vector;
import List;
import ForwardList;
import std;

using namespace dslab;
using namespace std;

class ContinuousPush : public Algorithm<void(size_t)> {
public:
    virtual void initialize() = 0;
};

template <template<typename> typename Linear>
    requires is_base_of_v<AbstractLinearList<int, typename Linear<int>::position_type>, Linear<int>>
class ContinuousPushBack : public ContinuousPush {
    Linear<int> m_container;
public:
    void initialize() override {
        m_container = Linear<int> {};
    }
    void operator()(size_t n) override {
        for (size_t i { 0 }; i < n; ++i) {
            m_container.push_back(i);
        }
    }
    string type_name() const override {
        return m_container.type_name();
    }
};

template <template<typename> typename Linear>
    requires is_base_of_v<AbstractLinearList<int, typename Linear<int>::position_type>, Linear<int>>
class ContinuousPushFront : public ContinuousPush {
    Linear<int> m_container;
public:
    void initialize() override {
        m_container = {};
    }
    void operator()(size_t n) override {
        if constexpr (is_base_of_v<AbstractVector<int>, Linear<int>>) {
            if (n > 100'000) {
                throw runtime_error { R"(Vector::push_front() is too slow for n > 100'000)" };
            }
        }
        for (size_t i { 0 }; i < n; ++i) {
            m_container.push_front(i);
        }
    }
    string type_name() const override {
        return m_container.type_name();
    }
};

vector testData { 10, 1000, 10'000, 100'000, 1'000'000, 3'000'000 };

TestFramework<ContinuousPush, ContinuousPushBack<DefaultVector>, ContinuousPushBack<List>, ContinuousPushBack<ForwardList>> test_back;
TestFramework<ContinuousPush, ContinuousPushFront<DefaultVector>, ContinuousPushFront<List>, ContinuousPushFront<ForwardList>> test_front;

template <typename TestType>
void runTest(TestType& test) {
    for (auto n : testData) {
        cout << format("n = {:>8} ", n) << endl;
        test.run([](auto& algo) { algo.initialize(); });
        test(n);
    }
}

int main() {
    cout << "Push Back Test" << endl;
    runTest(test_back);
    cout << "Push Front Test" << endl;
    runTest(test_front);
}