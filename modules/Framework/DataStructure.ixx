export module Framework.DataStructure;

import Framework.Object;

export namespace dslab {
template <typename T>
class DataStructure : public Object {
public:
    virtual size_t size() const = 0;
    virtual bool empty() const {
        return size() == 0;
    }
};

}