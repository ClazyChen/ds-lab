#include <vector>
#include <format>
#include <iostream>
#include <algorithm>
#include <random>
#include <numeric>
import Framework;
import Vector;
import Factorial;
using namespace dslab;
using namespace std;


class Shuffle : public Algorithm<void(Vector<int>&)> {};

class ShuffleBasic : public Shuffle {
public:
    void operator()(Vector<int>& V) override {
        for (auto i { V.size() }; i > 1; --i) {
            auto j { rand() % i };
            swap(V[i - 1], V[j]);
        }
    }
};

class ShuffleStd : public Shuffle {
    default_random_engine m_engine;
public:
    void operator()(Vector<int>& V) override {
        shuffle(V.begin(), V.end(), m_engine);
    }
};

class ShuffleTestProblem : public Algorithm<void()> {};

template <size_t N, size_t Times, typename Shuf>
    requires (N > 0 && Times > 0 && std::is_base_of_v<Shuffle, Shuf>)
class ShuffleTest : public ShuffleTestProblem {
    Vector<int> m_vector;
    Vector<int> m_counter;
    Factorial m_factorial;
    Shuf m_shuffle;

    int getRank() {
        auto rank { 0 };
        for (auto i { 0 }; i < N; ++i) {
            auto count { 0 };
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
        iota(begin(m_vector), end(m_vector), 0);
        m_counter.resize(m_factorial(N), 0);
        for (auto i { 0 }; i < Times; ++i) {
            m_shuffle(m_vector);
            ++m_counter[getRank()];
        }
        cout << endl;
        for (auto i { 0 }; i < m_counter.size(); ++i) {
            cout << format("{:3d}: {:14d} {:12.2f}%", i, m_counter[i], 100.0 * m_counter[i] / Times) << endl;
        }
        double avg { static_cast<double>(Times) / m_factorial(N) };
        double error { 0 };
        for (auto i { 0 }; i < m_counter.size(); ++i) {
            error += abs(m_counter[i] - avg);
        }
        error /= m_factorial(N);
        // TODO: 这里直接使用 std::format 会报错，需要调查
        // 怀疑是Visual Studio 2022存在的BUG
        cout << vformat("Average: {:10.2f} Error: {:5.2f}%", std::make_format_args(avg, 100.0 * error / avg)) << endl;
    }
};

TestFramework<ShuffleTestProblem,
    ShuffleTest<4, 1'000'000, ShuffleBasic>,
    ShuffleTest<4, 1'000'000, ShuffleStd>> test;
    
int main() {
    test();
    return 0;
}