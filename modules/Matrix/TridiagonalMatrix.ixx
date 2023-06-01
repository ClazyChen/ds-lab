export module Matrix.TridiagonalMatrix;

import Matrix.AbstractCompressedMatrix;
import std;

export namespace dslab {

template <typename T, size_t N> requires (N >= 0)
class TridiagonalMatrix : public AbstractCompressedMatrix<T, N, N, 3 * N - 2> {
    using Base = AbstractCompressedMatrix<T, N, N, 3 * N - 2>;
    T zero {};
protected:
    size_t index(size_t r, size_t c) const override {
        return 2 * r + c;
    }

public:
    T& get(size_t r, size_t c) override {
        if (std::abs(int(r) - int(c)) > 1) {
            throw std::out_of_range("Invalid index");
        }
        return Base::get(r, c);
    }

    const T& get(size_t r, size_t c) const override {
        if (std::abs(int(r) - int(c)) > 1) {
            return zero;
        }
        return Base::get(r, c);
    }

    TridiagonalMatrix() = default;
    TridiagonalMatrix(std::initializer_list<std::initializer_list<T>> ilist) {
        auto itr { std::begin(ilist) };
        for (size_t r { 0 }; r < N; ++r) {
            auto itc { std::begin(*itr++) };
            for (int c { -1 }; c <= 1; ++c) {
                if (r + c >= 0 && r + c < N) {
                    this->get(r, r + c) = *itc++;
                }
            }
        }
    }

    TridiagonalMatrix& operator=(std::initializer_list<std::initializer_list<T>> ilist) {
        TridiagonalMatrix tmp { ilist };
        std::swap(this->m_data, tmp.m_data);
        return *this;
    }
};

}