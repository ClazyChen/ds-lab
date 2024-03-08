#pragma once
#include "Algorithm.hpp"

namespace dslab::framework {
    class Gcd : public Algorithm<int(int, int)> {
    public:
        int operator()(int a, int b) override {
            while (b != 0) {
                int r { a % b };
                a = b;
                b = r;
            }
            return a;
        }
        std::string type_name() const override {
            return "Euclidean Gcd (Iterative)";
        }
    };
}