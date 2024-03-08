#include "framework.hpp"

class Case : public dslab::Object {
public:
    std::string type_name() const override {
        return "My Case";
    }
};

using namespace dslab;

int main() {
    Object* o = new Object();
    Object* c = new Case();
    std::cout << "Object: " << o->type_name() << std::endl; // "Undefined Object Name"
    std::cout << "Case: " << c->type_name() << std::endl; // "My Case"
    return 0;
}