export module Matrix.TripleMatrix;

import Matrix.AbstractMatrix;
import Triple;
import Vector;
import std;

export namespace dslab {

template <typename T, size_t R, size_t C = R>
    requires (R >= 0 && C >= 0)
class TripleMatrix : public AbstractMatrix<T, R, C> {
    Vector<Triple<T>> V;
    T zero {};
public:
    T& get(size_t r, size_t c) override {
        for (auto& t : V) {
            if (t.row() == r && t.col() == c) {
                return t.value();
            }
        }
        V.push_back(Triple<T> { r, c, T {} });
        return V.back().value();
    }
    const T& get(size_t r, size_t c) const override {
        for (const auto& t : V) {
            if (t.row() == r && t.col() == c) {
                return t.value();
            }
        }
        return zero;
    }

    void clear() override {
        V.clear();
    }

    TripleMatrix() = default;
    TripleMatrix(std::initializer_list<Triple<T>> ilist) : V { ilist } {}
    TripleMatrix& operator=(std::initializer_list<std::initializer_list<T>> ilist) {
        V = ilist;
        return *this;
    }

    std::string type_name() const override {
        return std::format("Matrix {}x{} (Triple)", R, C);
    }
};

}