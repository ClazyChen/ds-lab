module;
#include <array>

export module Matrix.RowMajorMatrix;

import Matrix.AbstractMatrix;

export namespace dslab {

template <typename T, size_t R, size_t C = R>
    requires (R >= 0 && C >= 0)
class RowMajorMatrix : public AbstractMatrix<T, R, C> {
    std::array<T, R * C> m_data;
public:
    T& get(size_t r, size_t c) override {
        return m_data[r * C + c];
    }
    const T& get(size_t r, size_t c) const {
        return m_data[r * C + c];
    }
    void clear() override {
        m_data.fill(T{});
    }
};

}