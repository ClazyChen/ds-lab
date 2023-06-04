import Framework;
import Vector;
import Factorial;
import std;

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
    string type_name() const override {
        return "Random Shuffle (explicit)";
    }
};

class ShuffleStd : public Shuffle {
    default_random_engine m_engine { random_device {}() };
public:
    void operator()(Vector<int>& V) override {
        shuffle(begin(V), end(V), m_engine);
    }
    string type_name() const override {
        return "Random Shuffle (std::shuffle)";
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
        cout << format("Average: {:10.2f} Error: {:5.2f}%", avg, 100.0 * error / avg) << endl;
    }

    string type_name() const override {
        return format("Shuffle Test (N = {}, Times = {}, {})", N, Times, m_shuffle.type_name());
    }
};

TestFramework<ShuffleTestProblem,
    ShuffleTest<4, 1'000'000, ShuffleBasic>,
    ShuffleTest<4, 1'000'000, ShuffleStd>> test;
    
int main() {
    test();
    return 0;
}