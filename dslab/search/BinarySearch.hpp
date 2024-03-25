#include "AbstractSearch.hpp"

namespace dslab::search {

template <typename T, template<typename> typename L>
requires std::is_base_of_v<AbstractVector<T>, L<T>>
class BinarySearch : public AbstractSearch<T, L> {
    using iterator = typename L<T>::iterator;
protected:
    iterator search(const L<T>& V, const T& e) override {
        auto lo { V.begin() }, hi { V.end() };
        while (lo < hi) {
            auto mi { lo + (hi - lo) / 2 };
            if (this->cmp(e, *mi)) {
                hi = mi;
            } else {
                lo = mi + 1;
            }
        }
        return lo - 1;
    }
};

}