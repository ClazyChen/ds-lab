module;
#include <type_traits>

export module Search.AbstractSearch;

import Framework.Algorithm;
import LinearList;
import Vector;

export namespace dslab {

template <typename T, template <typename> typename Linear = DefaultVector, typename Comparator = std::less<T>>
    requires std::is_base_of_v<AbstractLinearList<T, typename Linear<T>::position_type>, Linear<T>>
class AbstractSearch : public Algorithm<typename Linear<T>::position_type, const Linear<T>&, const T&> {};

}