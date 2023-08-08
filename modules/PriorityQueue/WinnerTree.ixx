export module PriorityQueue.WinnerTree;

import PriorityQueue.AbstractPriorityQueue;
import Vector;
import std;

export namespace dslab {

template <typename T>
class WinnerTree : public AbstractPriorityQueue<T> {
    Vector<T> V;

    constexpr static Rank left(Rank r) {
        return r * 2;
    }

    constexpr static Rank right(Rank r) {
        return r * 2 + 1;
    }

    constexpr static Rank parent(Rank r) {
        return r / 2;
    }

    bool update(Rank r) {
        auto m { this->cmp(V[left(r)], V[right(r)]) ? left(r) : right(r) };
        auto ret { V[m] != V[r] };
        V[r] = V[m];
        return ret;
    }

    void build(Vector<T>&& V0) {
        auto n { V0.size() };
        this->V.resize(n * 2);
        std::move(std::begin(V0), std::end(V0), std::begin(V) + n);
        for (Rank r { n - 1 }; r > 0; --r) {
            update(r);
        }
    }

public:
    WinnerTree() = default;
    WinnerTree(Vector<T>&& V, const std::function<bool(const T&, const T&)>& cmp = std::less<T>()) : AbstractPriorityQueue<T>(cmp) {
        build(std::move(V));
    }

    size_t size() const override {
        return V.size() / 2;
    }

    void push(const T& e) override {
        auto n { V.size() / 2 };
        auto r { n == 0 ? e : V[n] };
        V.push_back(r);
        V.push_back(e);
        for (Rank r { n }; r > 0; r = parent(r)) {
            if (!update(r)) {
                break;
            }
        }
    }

    void push(T&& e) override {
        auto n { V.size() / 2 };
        auto r { n == 0 ? e : V[n] };
        V.push_back(r); 
        V.push_back(std::move(e));
        for (Rank r { n }; r > 0; r = parent(r)) {
            if (!update(r)) {
                break;
            }
        }
    }

    T pop() override {
        auto n { V.size() / 2 };
        auto c { 1 };
        while (c < n) {
            if (V[c] == V[left(c)]) {
                c = left(c);
            } else {
                c = right(c);
            }
        }
        auto ret { V[c] };
        if (c == left(n - 1)) {
            V[c = n - 1] = std::move(V[right(n - 1)]);
        } else if (c == right(n - 1)) {
            V[c = n - 1] = std::move(V[left(n - 1)]);
        } else if (V[n - 1] == V[left(n - 1)]) {
            V[c] = std::move(V[right(n - 1)]);
        } else {
            V[c] = std::move(V[left(n - 1)]);
        }
        V.pop_back();
        V.pop_back();
        for (c = parent(c); c > 0; c = parent(c)) {
            update(c);
        }
        return ret;
    }

    const T& top() const override {
        return V[1];
    }

    template <typename T1>
    friend std::ostream& operator<<(std::ostream& os, const WinnerTree<T1>& t);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const WinnerTree<T>& t) {
    os << "WinnerTree(" << t.V << ")";
    return os;
}

}