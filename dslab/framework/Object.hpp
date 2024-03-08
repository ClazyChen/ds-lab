#pragma once
#include <string>

namespace dslab::framework {
    class Object {
    public:
        virtual std::string type_name() const {
            return "Undefined Object Name";
        }
    };
}