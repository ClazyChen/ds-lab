module;
#include <array>

export module Matrix.ColumnMajorMatrix;

import Matrix.AbstractMatrix;

export namespace dslab {

template <typename T, size_t R, size_t C = R>
    requires (R >= 0 && C >= 0)
class ColumnMajorMatrix : public AbstractMatrix<T, R, C> {
    std::array<T, R * C> m_data;
public:
    T& get(size_t r, size_t c) override {
        return m_data[c * R + r];
    }
    const T& get(size_t r, size_t c) const override {
        return m_data[c * R + r];
    }
};

}