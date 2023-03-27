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
    RowMajorMatrix() = default;
    
    RowMajorMatrix(std::initializer_list<T> ilist) {
        auto itr { std::begin(ilist) };
        for (size_t r { 0 }; r < R; ++r) {
            auto itc { std::begin(*itr++) };
            for (size_t c { 0 }; c < C; ++c) {
                m_data[r * C + c] = *itc++;
            }
        }
    }

    RowMajorMatrix& operator=(std::initializer_list<T> ilist) {
        RowMajorMatrix tmp { ilist };
        std::swap(m_data, tmp.m_data);
        return *this;
    }

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