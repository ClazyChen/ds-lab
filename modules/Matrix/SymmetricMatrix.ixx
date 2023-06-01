export module Matrix.SymmetricMatrix;

import Matrix.AbstractCompressedMatrix;
import std;

export namespace dslab {

template <typename T, size_t N> requires (N >= 0)
class SymmetricMatrix : public AbstractCompressedMatrix<T, N, N, N * (N + 1) / 2> {
protected:
    size_t index(size_t r, size_t c) const override {
        return (r < c) ? c * (c + 1) / 2 + r : r * (r + 1) / 2 + c;
    }

public:
    SymmetricMatrix() = default;
    SymmetricMatrix(std::initializer_list<std::initializer_list<T>> ilist) {
        auto itr { std::begin(ilist) };
        for (size_t r { 0 }; r < N; ++r) {
            auto itc { std::begin(*itr++) };
            for (size_t c { 0 }; c < N; ++c) {
                this->get(r, c) = *itc++;
            }
        }
    }

    SymmetricMatrix& operator=(std::initializer_list<std::initializer_list<T>> ilist) {
        SymmetricMatrix tmp { ilist };
        std::swap(this->m_data, tmp.m_data);
        return *this;
    }
};

}