module;
#include <type_traits>
#include <functional>

export module Search.AbstractSearch;

import Framework.Algorithm;
import LinearList;
import Vector;

export namespace dslab {

template <typename T, template <typename> typename Linear = DefaultVector>
    requires std::is_base_of_v<AbstractLinearList<T, typename Linear<T>::position_type>, Linear<T>>
class AbstractSearch : public Algorithm<typename Linear<T>::position_type(const Linear<T>&, const T&)> {
protected:
    using output_type = typename Linear<T>::position_type;
    std::function<bool(const T&, const T&)> cmp { std::less<T>() };
    virtual output_type search(const Linear<T>& L, const T& e) = 0;
public:
    template <typename Comparator>
    output_type operator()(const Linear<T>& L, const T& e, Comparator cmp) {
        this->cmp = cmp;
        return search(L, e);
    }
    output_type operator()(const Linear<T>& L, const T& e) override {
        return search(L, e);
    }
};

}