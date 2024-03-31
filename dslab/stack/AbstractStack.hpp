#pragma once

#include "../../framework.hpp"

namespace dslab::stack {

    template <typename T>
    class AbstractStack {
    public:
        using value_type = T;

        virtual void push(const T& e) = 0;
        virtual void push(T&& e) = 0;
        virtual void pop() = 0;
        virtual T& top() = 0;
    };

}