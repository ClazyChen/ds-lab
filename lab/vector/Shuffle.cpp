#include "vector.hpp"

using namespace dslab;

template <typename T>
using Shuffle = Algorithm<void(Vector<T>&)>;

template <typename T>
class ShuffleBasic : public Shuffle<T> {
public:
    void operator()(Vector<T>& V) override {
        for (auto i { V.size() - 1 }; i > 0; --i) {
            auto j { Random::get(i) };
            std::swap(V[i], V[j]);
        }
    }
    std::string type_name() const override {
        return "Random Shuffle (explicit)";
    }
};

template <typename T>
class ShuffleStd : public Shuffle<T> {
public:
    void operator()(Vector<T>& V) override {
        std::shuffle(V.begin(), V.end(), Random::engine());
    }
    std::string type_name() const override {
        return "Random Shuffle (std::shuffle)";
    }
};

using ShuffleTestProblem = Algorithm<void()>;

template <std::size_t N, std::size_t C, typename T, template<typename> typename S>
    requires (N > 0 && C > 0 && std::is_base_of_v<Shuffle<T>, S<T>>)
class ShuffleTest : public ShuffleTestProblem {
    Vector<T> m_vector;
    Vector<std::size_t> m_counter;
    Factorial m_factorial;
    S<T> m_shuffle;

    std::size_t getRank() {
        auto rank { 0uz };
        for (auto i { 0uz }; i < N; ++i) {
            auto count { 0uz };
            for (auto j { i + 1 }; j < N; ++j) {
                if (m_vector[i] > m_vector[j]) {
                    ++count;
                }
            }
            rank += count * m_factorial(N - i - 1);
        }
        return rank;
    }

public:
    void operator()() override {
        m_vector.resize(N);
        std::iota(m_vector.begin(), m_vector.end(), 0);
        m_counter.resize(m_factorial(N));
        std::fill(m_counter.begin(), m_counter.end(), 0);
        for (auto i { 0uz }; i < C; ++i) {
            m_shuffle(m_vector);
            ++m_counter[getRank()];
        }
        std::cout << std::endl;
        for (auto i { 0uz }; i < m_counter.size(); ++i) {
            std::cout << std::format("{:3}: {:6} ({:6.2f}%)", i, m_counter[i], 100.0 * m_counter[i] / C) << std::endl;
        }
        auto avg { static_cast<double>(C) / m_factorial(N) }, error { 0.0 };
        for (auto c : m_counter) {
            error += std::abs(c - avg);
        }
        error /= m_factorial(N);
        std::cout << std::format("Average: {:6.2f} Error: {:6.2f}", avg, error) << std::endl;
    }

    std::string type_name() const override {
        return std::format("Shuffle Test (N = {}, C = {}, {})", N, C, m_shuffle.type_name());
    }
};

TestFramework<ShuffleTestProblem,
    ShuffleTest<4, 100'0000, int, ShuffleBasic>,
    ShuffleTest<4, 100'0000, int, ShuffleStd>> test;

int main() {
    test();
    return 0;
}