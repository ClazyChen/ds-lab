module;
#include <utility>

export module Matrix.AbstractMatrix;

import Framework.DataStructure;

export namespace dslab {

template <typename T, size_t R, size_t C>
    requires (R >= 0 && C >= 0)
class AbstractMatrix : public DataStructure<T> {
public:
    consteval size_t rows() const { return R; }
    consteval size_t cols() const { return C; }
    virtual T& get(size_t r, size_t c) = 0;
    virtual const T& get(size_t r, size_t c) const = 0;
    virtual void set(size_t r, size_t c, const T& e) {
        get(r, c) = e;
    }
    virtual void set(size_t r, size_t c, T&& e) {
        get(r, c) = std::move(e);
    }
    size_t size() const override {
        return R * C;
    }
    virtual void clear() = 0;
    T& operator[](std::pair<size_t, size_t> p) {
        return get(p.first, p.second);
    }
    const T& operator[](std::pair<size_t, size_t> p) const {
        return get(p.first, p.second);
    }
};

}