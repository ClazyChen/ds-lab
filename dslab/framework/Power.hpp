#pragma once
#include "Algorithm.hpp"

namespace dslab::framework {

    class Power : public Algorithm<int(int, int)> {
    public:
        int operator()(int a, int b) override {
            int result { 1 };
            while (b > 0) {
                if (b % 2 == 1) {
                    result *= a;
                }
                a *= a;
                b /= 2;
            }
            return result;
        }
        std::string type_name() const override {
            return "Exponent by squaring (Iterative)";
        }
    };

}