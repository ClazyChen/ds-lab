module;
#include <stdexcept>

export module Matrix.TriangularMatrix;

import Matrix.AbstractCompressedMatrix;

export namespace dslab {

template <typename T, size_t N> requires (N >= 0)
class LowerTriangularMatrix : public AbstractCompressedMatrix<T, N, N, N* (N + 1) / 2> {
    using Base = AbstractCompressedMatrix<T, N, N, N* (N + 1) / 2>;
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
            return T {};
        }
        return Base::get(r, c);
    }
    void set(size_t r, size_t c, const T& e) override {
        if (r < c) {
            throw std::out_of_range("Invalid index");
        }
        Base::set(r, c, e);
    }
    void set(size_t r, size_t c, T&& e) override {
        if (r < c) {
            throw std::out_of_range("Invalid index");
        }
        Base::set(r, c, std::move(e));
    }
};

template <typename T, size_t N> requires (N >= 0)
class UpperTriangularMatrix : public AbstractCompressedMatrix<T, N, N, N* (N + 1) / 2> {
    using Base = AbstractCompressedMatrix<T, N, N, N* (N + 1) / 2>;
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
            return T {};
        }
        return Base::get(r, c);
    }
    void set(size_t r, size_t c, const T& e) override {
        if (r > c) {
            throw std::out_of_range("Invalid index");
        }
        Base::set(r, c, e);
    }
    void set(size_t r, size_t c, T&& e) override {
        if (r > c) {
            throw std::out_of_range("Invalid index");
        }
        Base::set(r, c, std::move(e));
    }
};

}