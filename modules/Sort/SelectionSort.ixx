export module Sort.SelectionSort;

import Vector;
import Sort.AbstractSort;
import std;

export namespace dslab {

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractVector<T>, Linear<T>>
class SelectionSort : public AbstractSort<T, Linear> {
protected:
    void sort(Linear<T>& V) override {
        for (Rank i { 0 }; i < V.size(); ++i) {
            auto j { std::min_element(begin(V) + i, end(V), this->cmp) - begin(V) };
            std::swap(V[i], V[j]);
        }
    }
public:
    std::string type_name() const override {
        return "Selection Sort";
    }
};

}