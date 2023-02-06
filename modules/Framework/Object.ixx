module;
#include <string>
#include <typeinfo>

export module Framework.Object;

export namespace dslab {
class Object {
public:
    virtual std::string type_name() const {
#ifdef _MSC_VER
        return std::string { typeid(*this).name() }.substr(6);
#else
        return typeid(*this).name();
#endif
    }
};
}