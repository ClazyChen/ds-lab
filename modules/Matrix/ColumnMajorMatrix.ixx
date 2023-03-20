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
    ColumnMajorMatrix() = default;

    ColumnMajorMatrix(std::initializer_list<T> ilist) {
        auto itr { std::begin(ilist) };
        for (size_t r { 0 }; r < R; ++r) {
            auto itc { std::begin(*itr++) };
            for (size_t c { 0 }; c < C; ++c) {
                m_data[c * R + r] = *itc++;
            }
        }
    }

    ColumnMajorMatrix& operator=(std::initializer_list<T> ilist) {
        ColumnMajorMatrix tmp { ilist };
        std::swap(m_data, tmp.m_data);
        return *this;
    }

    T& get(size_t r, size_t c) override {
        return m_data[c * R + r];
    }
    const T& get(size_t r, size_t c) const {
        return m_data[c * R + r];
    }
    void set(size_t r, size_t c, const T& e) override {
        m_data[c * R + r] = e;
    }
    void set(size_t r, size_t c, T&& e) override {
        m_data[c * R + r] = std::move(e);
    }
    consteval size_t size() const override {
        return R * C;
    }
};

}