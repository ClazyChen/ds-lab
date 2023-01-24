export module Framework.Algorithm;

import Framework.Object;

export namespace dslab {

template <typename OutputType, typename... InputTypes>
class Algorithm : public Object {
public:
    using Output = OutputType;
    virtual OutputType operator()(InputTypes... inputs) = 0;
};

}