export module Sort.AbstractSort;

import Framework.Algorithm;
import LinearList;
import std;

export namespace dslab {

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractLinearList<T, typename Linear<T>::position_type>, Linear<T>>
class AbstractSort : public Algorithm<void(Linear<T>&)> {
protected:
    std::function<bool(const T&, const T&)> cmp { std::less<T>() };
    virtual void sort(Linear<T>& L) = 0;
public:
    template <typename Comparator>
    void operator()(Linear<T>& L, Comparator cmp) {
        this->cmp = cmp;
        sort(L);
    }
    void operator()(Linear<T>& L) override {
        sort(L);
    }
};

}