module;
#include <algorithm>

export module Sort.InsertionSort;

import Vector;
import Sort.AbstractSort;

export namespace dslab {

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractVector<T>, Linear<T>>
class InsertionSort : public AbstractSort<T, Linear> {
protected:
    void sort(Linear<T>& V) override {
        Rank n { V.size() };
        for (Rank i { 1 }; i < n; ++i) {
            Rank j { i };
            if (this->cmp(V[i], V[i - 1])) {
                T tmp { std::move(V[i]) };
                do {
                    V[j] = std::move(V[j - 1]);
                } while (--j > 0 && this->cmp(tmp, V[j - 1]));
                V[j] = std::move(tmp);
            }
        }
    }
};

}