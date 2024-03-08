#pragma once
#include "Object.hpp"

namespace dslab::framework {
    template <typename T>
    class DataStructure : public Object {
    public:
        virtual std::size_t size() const = 0;
        virtual bool empty() const {
            return size() == 0;
        }
    };
}
