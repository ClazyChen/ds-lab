#pragma once

namespace dslab::framework {
    template <typename OutputType, typename... InputTypes>
    class Algorithm;

    template <typename OutputType, typename... InputTypes>
    class Algorithm<OutputType(InputTypes...)> : public Object {
    public:
        using Output = OutputType;
        template <std::size_t N>
        using Input = std::tuple_element_t<N, std::tuple<InputTypes...>>;
        virtual OutputType operator()(InputTypes... inputs) = 0;
    };
}
