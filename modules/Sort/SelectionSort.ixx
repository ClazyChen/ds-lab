export module Sort.SelectionSort;

import Vector;
import List;
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

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractList<T>, Linear<T>>
class ListSelectionSort : public AbstractSort<T, Linear> {
    using It = Linear<T>::iterator;
protected:
    void sort(Linear<T>& L) override {
        for (auto i { std::begin(L) }; i != std::end(L); ) {
            if (auto j { std::min_element(i, std::end(L), this->cmp)}; i != j) {
                L.insertAsPrev(i, L.remove(j));
            } else {
                i = std::next(i);
            }
        }
    }
public:
    std::string type_name() const override {
        return "Selection Sort / List";
    }
};

}