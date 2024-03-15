#pragma once

#include <random>

namespace dslab::framework {

class Random {
protected:
    static std::mt19937 m_generator;
    static std::uniform_int_distribution<std::size_t> m_distribution;

public:

    // returns a random number in the range [min, max]
    static std::size_t get(std::size_t min, std::size_t max) {
        return m_distribution(m_generator, std::uniform_int_distribution<std::size_t>::param_type(min, max));
    }
    static std::size_t get(std::size_t max) {
        return get(0, max);
    }
    static std::size_t get() {
        return get(0, std::numeric_limits<std::size_t>::max());
    }
    static void seed(std::size_t value) {
        m_generator.seed(value);
    }
    static void seed() {
        m_generator.seed(std::random_device{}());
    }
};

std::mt19937 Random::m_generator { std::random_device{}() };
std::uniform_int_distribution<std::size_t> Random::m_distribution;

}