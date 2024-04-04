#pragma once

#include "stack.hpp"

namespace dslab::stack {

template <typename L = std::string>
class RandomStack : public Algorithm<L(std::size_t, const typename L::value_type&, const typename L::value_type&)> {
    using iterator = typename L::iterator;
    using T = typename L::value_type;

public:
    L operator()(std::size_t n, const T& push, const T& pop) override {
        L v {};
        auto p { n }, q { n };
        for (auto i { 0uz }; i < n * 2; ++i) {
            auto r { Random::get((p + q) * (q - p + 1) - 1) };
            if (r < p * (q - p + 2)) {
                v.push_back(push);
                --p;
            } else {
                v.push_back(pop);
                --q;
            }
        }
        return v;
    }

};

}