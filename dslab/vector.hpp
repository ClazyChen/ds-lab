#pragma once

#include "vector/AbstractVector.hpp"
#include "vector/VectorAllocator.hpp"
#include "vector/LinearList.hpp"
#include "vector/Vector.hpp"
#include "vector/VectorIterator.hpp"
#include "vector/VectorFormatter.hpp"

namespace dslab {
    using namespace vector;

    template <typename T>
    using DefaultVector = Vector<T, VectorAllocatorGP<std::ratio<3, 2>>>;
}