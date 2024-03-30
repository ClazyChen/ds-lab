#pragma once

#include "../vector.hpp"

namespace dslab::sort {

template <typename T, template<typename> typename L = DefaultVector>
requires std::is_base_of_v<LinearList<T, typename L<T>::iterator, typename L<T>::const_iterator>, L<T>>
class AbstractSort : public Algorithm<void(L<T>&)> {
protected:
    std::function<bool(const T&, const T&)> cmp { std::less<T>() };
    virtual void sort(L<T>& V) = 0;
public:
    template <typename Comparator>
    void operator()(L<T>& V, Comparator&& cmp) {
        this->cmp = cmp;
        sort(V);
    }
    void operator()(L<T>& V) override {
        sort(V);
    }
};

}