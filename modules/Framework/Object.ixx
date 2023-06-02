export module Framework.Object;

import std;

export namespace dslab {

// 每一个会被输出的实例都应当重载命名函数
class Object {
public:
    virtual std::string type_name() const {
#ifdef _MSC_VER
        return "Undefined Object Name";
        // TODO: in MSVC, type_info is not in module std
        //       and it may cause error if both use "import std" and import <typeinfo> 
        //       so the object name is not available at this time
        // return std::string { typeid(*this).name() }.substr(6);
#else
        return typeid(*this).name();
#endif
    }
};
}