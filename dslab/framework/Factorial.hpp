#pragma once

#include "Algorithm.hpp"

namespace dslab::framework {

template <typename T>
class Factorial : public Algorithm<T(T)> {
    constexpr T factorial(T n) const {
        return n == 0 ? T { 1 } : n * factorial(n - 1);
    }
public:
    T operator()(T n) override {
        return factorial(n);
    }
    T operator()(T n) const {
        return factorial(n);
    }
    std::string type_name() const override {
        return "Factorial (constexpr)";
    }
};

}