#pragma once
#include "Algorithm.hpp"

namespace dslab::framework {

    template <typename T>
    class Gcd : public Algorithm<T(T, T)> {
    public:
        T operator()(T a, T b) override {
            while (b != 0) {
                auto r { a % b };
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