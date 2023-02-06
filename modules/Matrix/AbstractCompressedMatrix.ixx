module;
#include <array>

export module Matrix.AbstractCompressedMatrix;

import Matrix.AbstractMatrix;

export namespace dslab {

template <typename T, size_t R, size_t C, size_t D>
    requires (R >= 0 && C >= 0 && D >= 0)
class AbstractCompressedMatrix : public AbstractMatrix<T, R, C> {
    std::array<T, D> m_data;
protected:
    virtual size_t index(size_t r, size_t c) const = 0;
public:
    consteval size_t size() const override {
        return D;
    }
    T& get(size_t r, size_t c) override {
        return m_data[index(r, c)];
    }
    void set(size_t r, size_t c, const T& e) override {
        m_data[index(r, c)] = e;
    }
    void set(size_t r, size_t c, T&& e) override {
        m_data[index(r, c)] = std::move(e);
    }
};

}