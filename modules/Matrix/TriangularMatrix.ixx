export module Matrix.TriangularMatrix;

import Matrix.AbstractCompressedMatrix;
import std;

export namespace dslab {

template <typename T, size_t N> requires (N >= 0)
class LowerTriangularMatrix : public AbstractCompressedMatrix<T, N, N, N* (N + 1) / 2> {
    using Base = AbstractCompressedMatrix<T, N, N, N* (N + 1) / 2>;
    T zero {};
protected:
    size_t index(size_t r, size_t c) const override {
        return r * (r + 1) / 2 + c;
    }

public:
    T& get(size_t r, size_t c) override {
        if (r < c) {
            throw std::out_of_range("Invalid index");
        }
        return Base::get(r, c);
    }

    const T& get(size_t r, size_t c) const override {
        if (r < c) {
            return zero;
        }
        return Base::get(r, c);
    }

    LowerTriangularMatrix() = default;
    LowerTriangularMatrix(std::initializer_list<std::initializer_list<T>> ilist) {
        auto itr { std::begin(ilist) };
        for (size_t r { 0 }; r < N; ++r) {
            auto itc { std::begin(*itr++) };
            for (size_t c { 0 }; c <= r; ++c) {
                this->get(r, c) = *itc++;
            }
        }
    }

    LowerTriangularMatrix& operator=(std::initializer_list<std::initializer_list<T>> ilist) {
        LowerTriangularMatrix tmp { ilist };
        std::swap(this->m_data, tmp.m_data);
        return *this;
    }
};

template <typename T, size_t N> requires (N >= 0)
class UpperTriangularMatrix : public AbstractCompressedMatrix<T, N, N, N* (N + 1) / 2> {
    using Base = AbstractCompressedMatrix<T, N, N, N* (N + 1) / 2>;
    T zero {};
protected:
    size_t index(size_t r, size_t c) const override {
        return (r * (2 * N - 1 - r)) / 2 + c;
    }

public:
    T& get(size_t r, size_t c) override {
        if (r > c) {
            throw std::out_of_range("Invalid index");
        }
        return Base::get(r, c);
    }

    const T& get(size_t r, size_t c) const override {
        if (r > c) {
            return zero;
        }
        return Base::get(r, c);
    }

    UpperTriangularMatrix() = default;
    UpperTriangularMatrix(std::initializer_list<std::initializer_list<T>> ilist) {
        auto itr { std::begin(ilist) };
        for (size_t r { 0 }; r < N; ++r) {
            auto itc { std::begin(*itr++) };
            for (size_t c { r }; c < N; ++c) {
                this->get(r, c) = *itc++;
            }
        }
    }

    UpperTriangularMatrix& operator=(std::initializer_list<std::initializer_list<T>> ilist) {
        UpperTriangularMatrix tmp { ilist };
        std::swap(this->m_data, tmp.m_data);
        return *this;
    }
};

}