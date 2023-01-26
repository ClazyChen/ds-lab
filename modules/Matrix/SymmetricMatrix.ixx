module;
#include <utility>

export module Matrix.SymmetricMatrix;

import Matrix.AbstractCompressedMatrix;

export namespace dslab {

template <typename T, size_t N> requires (N >= 0)
class SymmetricMatrix : public AbstractCompressedMatrix<T, N, N, N*(N+1)/2> {
protected:
    size_t index(size_t r, size_t c) const override {
        return (r < c) ? c * (c + 1) / 2 + r : r * (r + 1) / 2 + c;
    }
};

}