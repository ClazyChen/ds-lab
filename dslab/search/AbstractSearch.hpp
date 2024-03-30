#pragma once

#include "../vector.hpp"

namespace dslab::search {

template <typename T, template <typename> typename L = DefaultVector>
requires std::is_base_of_v<LinearList<T, typename L<T>::iterator, typename L<T>::const_iterator>, L<T>>
class AbstractSearch : public Algorithm<typename L<T>::iterator(const L<T>&, const T&)> {
protected:
    using iterator = typename L<T>::iterator;
    std::function<bool(const T&, const T&)> cmp { std::less<T>() };
    virtual iterator search(const L<T>& V, const T& e) = 0;
public:
    template <typename Comparator>
    iterator operator()(const L<T>& V, const T& e, Comparator&& cmp) {
        this->cmp = cmp;
        return search(V, e);
    }
    iterator operator()(const L<T>& V, const T& e) override {
        return search(V, e);
    }
};

}