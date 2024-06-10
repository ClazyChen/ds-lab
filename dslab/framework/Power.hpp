#pragma once
#include "Algorithm.hpp"

namespace dslab::framework {

    template <typename T>
    class Power : public Algorithm<T(T, T)> {
    public:
        T operator()(T a, T b) override {
            return const_cast<const Power<T>&>(*this)(a, b);
        }
        T operator()(T a, T b) const {
            T result { 1 };
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