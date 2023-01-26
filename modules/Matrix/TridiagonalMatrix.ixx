module;
#include <cmath>
#include <stdexcept>

export module Matrix.TridiagonalMatrix;

import Matrix.AbstractCompressedMatrix;

export namespace dslab {

template <typename T, size_t N> requires (N >= 0)
class TridiagonalMatrix : public AbstractCompressedMatrix<T, N, N, 3 * N - 2> {
    using Base = AbstractCompressedMatrix<T, N, N, 3 * N - 2>;
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
            return T {};
        }
        return Base::get(r, c);
    }
    void set(size_t r, size_t c, const T& e) override {
        if (std::abs(int(r) - int(c)) > 1) {
            throw std::out_of_range("Invalid index");
        }
        Base::set(r, c, e);
    }
    void set(size_t r, size_t c, T&& e) override {
        if (std::abs(int(r) - int(c)) > 1) {
            throw std::out_of_range("Invalid index");
        }
        Base::set(r, c, std::move(e));
    }
};

}