module;
#include <utility>
#include <concepts>

export module Matrix.TripleMatrix;

import Matrix.AbstractMatrix;
import Triple;
import ForwardList;

export namespace dslab {

template <typename T, size_t R, size_t C = R, template<typename>typename Lis = ForwardList>
    requires (R >= 0 && C >= 0) && std::is_base_of_v<AbstractForwardList<Triple<T>>, Lis<Triple<T>>>
class TripleMatrix : public AbstractMatrix<T, R, C> {
    Lis<Triple<T>> L;
    using Pos = typename Lis<Triple<T>>::position_type;
    Triple<T>& find(size_t r, size_t c) {
        while (auto p { L.first() }; !L.end(p)) {
            auto& t { L.get(p) };
            if (t.row() == r && t.col() == c) {
                return t;
            } else if (t.value()) {
                p = L.next(p);
            } else {
                auto tmp { L.next(p) };
                L.remove(p);
                p = tmp;
            }
        }
        L.push_front(Triple<T> { r, c, T {} });
        return L.front();
    }
public:
    T& get(size_t r, size_t c) override {
        return find(r, c).value();
    }
    const T& get(size_t r, size_t c) const override {
        for (const auto& t : L) {
            if (t.row() == r && t.col() == c) {
                return t.value();
            }
        }
        return T {};
    }
    void set(size_t r, size_t c, const T& e) override {
        find(r, c).value() = e;
    }
    void set(size_t r, size_t c, T&& e) override {
        find(r, c).value() = std::move(e);
    }
    consteval size_t size() const override {
        return R * C;
    }
};

}