#pragma once

#include "Algorithm.hpp"

namespace dslab::framework {
    class Euclidx : public Algorithm<std::pair<int, int>(int, int)> {
    public:
        std::pair<int, int> operator()(int a, int b) override {
            int x { 0 }, y { 1 }, u { 1 }, v { 0 };
            while (a != 0) {
                int q { b / a }, r { b % a };
                int m { x - u * q }, n { y - v * q };
                b = a; a = r; x = u; y = v; u = m; v = n;
            }
            return { b, x };
        }
        std::string type_name() const override {
            return "Euclidean Index (Iterative)";
        }
    };
}