export module Stack.RandomStackOperation;

import Framework.Algorithm;
import Vector;
import std;

export namespace dslab {

template <typename T, typename Vec>
    requires std::is_base_of_v<AbstractVector<T>, Vec> || (std::is_same_v<T, char> && std::is_same_v<Vec, std::string>)
class RandomStackOperation : public Algorithm<Vec(size_t, const T&, const T&)> {
protected:
    std::default_random_engine m_engine { std::random_device {}() };
    std::uniform_int_distribution<size_t> m_distribution { 0, std::numeric_limits<size_t>::max() };

    size_t rand(size_t n) {
        return m_distribution(m_engine) % n;
    }

public:
    Vec operator()(size_t n, const T& push, const T& pop) override {
        Vec vec {};
        vec.resize(n * 2);
        size_t p { n }, q { n };
        for (size_t i { 0 }; i < n * 2; ++i) {
            size_t r { rand((p + q) * (q - p + 1)) };
            if (r < p * (q - p + 2)) {
                vec[i] = push;
                --p;
            } else {
                vec[i] = pop;
                --q;
            }
        }
        return vec;
    }
};

}
