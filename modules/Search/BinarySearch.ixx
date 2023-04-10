module;
#include <type_traits>

export module Search.BinarySearch;

import Search.AbstractSearch;
import Vector.AbstractVector;

export namespace dslab {

template <typename T, template<typename> typename Vec>
    requires std::is_base_of_v<AbstractVector<T>, Vec<T>>
class BinarySearch : public AbstractSearch<T, Vec> {
protected:
    Rank search(const Vec<T>& V, const T& e) override {
        Rank lo { 0 }, hi { V.size() };
        while (lo < hi) {
            Rank mi { lo + (hi - lo) / 2 };
            if (this->cmp(e, V[mi])) {
                hi = mi;
            } else {
                lo = mi + 1;
            }
        }
        return lo - 1;
    }
};

}