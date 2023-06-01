export module Matrix.AbstractCompressedMatrix;

import Matrix.AbstractMatrix;
import std;

export namespace dslab {

template <typename T, size_t R, size_t C, size_t D>
    requires (R >= 0 && C >= 0 && D >= 0)
class AbstractCompressedMatrix : public AbstractMatrix<T, R, C> {
protected:
    std::array<T, D> m_data;
    virtual size_t index(size_t r, size_t c) const = 0;
public:
    consteval size_t df() const { return D; }
    size_t size() const override {
        return D;
    }
    T& get(size_t r, size_t c) override {
        return m_data[index(r, c)];
    }
    const T& get(size_t r, size_t c) const override {
        return m_data[index(r, c)];
    }
    void clear() override {
        m_data.fill(T{});
    }
};

}