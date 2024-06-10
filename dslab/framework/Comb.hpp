#pragma once

#include "Algorithm.hpp"

namespace dslab::framework {

    template <typename T>
    class Comb : public Algorithm<T(T, T)> {
    protected:
        std::vector<std::vector<T>> mem;
        T comb(T n, T k) {
            if (mem[n][k] == 0) {
                mem[n][k] = comb(n, k - 1) * static_cast<std::size_t>(n - k + 1) / k;
            }
            return mem[n][k];
        }
    public:
        T operator()(T n, T k) override {
            while (n >= mem.size()) {
                mem.push_back(std::vector<T>(mem.size() + 1, 0));
                mem.back()[0] = 1;
            }
            return comb(n, k);
        }
        std::string type_name() const override {
            return "Comb (Memory Search 1D)";
        }
    };

}