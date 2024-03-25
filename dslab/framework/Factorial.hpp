#pragma once

#include "Algorithm.hpp"

namespace dslab::framework {

class Factorial : public Algorithm<int(int)> {
    constexpr int factorial(int n) const {
        return n == 0 ? 1 : n * factorial(n - 1);
    }
public:
    int operator()(int n) override {
        return factorial(n);
    }
    std::string type_name() const override {
        return "Factorial (constexpr)";
    }
};

}