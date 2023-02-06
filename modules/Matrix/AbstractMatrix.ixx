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
    virtual const T& get(size_t r, size_t c) const {
        return const_cast<AbstractMatrix*>(this)->get(r, c);
    }
    virtual void set(size_t r, size_t c, const T& e) = 0;
    virtual void set(size_t r, size_t c, T&& e) = 0;
};

}