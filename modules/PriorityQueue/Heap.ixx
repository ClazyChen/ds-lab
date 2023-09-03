export module PriorityQueue.Heap;

import PriorityQueue.AbstractPriorityQueue;
import Vector;
import std;

export namespace dslab {

template <typename T, bool NoShift = false>
class Heap : public AbstractPriorityQueue<T> {
    Vector<T> V;

    constexpr static Rank left(Rank r) {
        if constexpr (NoShift) {
            return r * 2 + 1;
        } else {
            return r * 2;
        }
    }

    constexpr static Rank right(Rank r) {
        if constexpr (NoShift) {
            return r * 2 + 2;
        } else {
            return r * 2 + 1;
        }
    }

    constexpr static Rank parent(Rank r) {
        if constexpr (NoShift) {
            return (r - 1) / 2;
        } else {
            return r / 2;
        }
    }

    constexpr static Rank root() {
        if constexpr (NoShift) {
            return 0;
        } else {
            return 1;
        }
    }

    constexpr static bool isRoot(Rank r) {
        return r == root();
    }

    void siftUp(Rank r) {
        while (!isRoot(r) && this->cmp(V[r], V[parent(r)])) {
            std::swap(V[r], V[parent(r)]);
            r = parent(r);
        }
    }

    void siftDown(Rank r) {
        while (true) {
            auto m { r };
            if (left(r) < V.size() && this->cmp(V[left(r)], V[m])) {
                m = left(r);
            }
            if (right(r) < V.size() && this->cmp(V[right(r)], V[m])) {
                m = right(r);
            }
            if (m == r) {
                break;
            }
            std::swap(V[r], V[m]);
            r = m;
        }
    }

    void build(Vector<T>&& V0) {
        auto n { V0.size() };
        this->V.resize(n + root());
        if constexpr (NoShift) {
            this->V = std::move(V0);
        } else {
            std::move(std::begin(V0), std::end(V0), std::begin(V) + 1);
        }
        if (n > 1) {
            for (auto r { parent(n + root() - 1) }; ; --r) {
                siftDown(r);
                if (r == root()) {
                    break;
                }
            }
        }
    }

public:
    Heap() : AbstractPriorityQueue<T>() {
        V.resize(root());
    }
    Heap(Vector<T>&& V, const std::function<bool(const T&, const T&)>& cmp = std::less<T>()) : AbstractPriorityQueue<T>(cmp) {
        build(std::move(V));
    }

    size_t size() const override {
        return V.size() - root();
    }

    void push(const T& e) override {
        V.push_back(e);
        siftUp(V.size() - 1);
    }

    void push(T&& e) override {
        V.push_back(std::move(e));
        siftUp(V.size() - 1);
    }

    T pop() override {
        auto ret { std::move(V[root()])};
        V[root()] = std::move(V[V.size() - 1]);
        V.pop_back();
        siftDown(root());
        return ret;
    }

    const T& top() const override {
        return V[root()];
    }

    template <typename T1, bool NoShift1>
    friend std::ostream& operator<<(std::ostream& os, const Heap<T1, NoShift1>& t);

};

template <typename T, bool NoShift>
std::ostream& operator<<(std::ostream& os, const Heap<T, NoShift>& t) {
    os << "Heap(" << t.V << ")";
    return os;
}

}