module;
#include <type_traits>

export module Sort.AbstractSort;

import Framework.Algorithm;
import LinearList;

export namespace dslab {

template <typename T, template<typename> typename Linear, typename Comparator = std::less<T>>
    requires std::is_base_of_v<AbstractLinearList<T, typename Linear<T>::position_type>, Linear<T>>
class AbstractSort : public Algorithm<void, Linear<T>&> {};

}