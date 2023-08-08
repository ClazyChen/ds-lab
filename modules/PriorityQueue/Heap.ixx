export module PriorityQueue.Heap;

import PriorityQueue.AbstractPriorityQueue;
import Vector;
import std;

export namespace dslab {

template <typename T>
class Heap : public AbstractPriorityQueue<T> {
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

    void siftUp(Rank r) {
        while (r > 1 && this->cmp(V[r], V[parent(r)])) {
            std::swap(V[r], V[parent(r)]);
            r = parent(r);
        }
    }

    void siftDown(Rank r) {
        if (left(r) >= V.size()) {
            return;
        }
        if (right(r) >= V.size() && this->cmp(V[left(r)], V[r])) {
            std::swap(V[r], V[left(r)]);
            return;
        }
        while (right(r) < V.size()) {
            auto m { this->cmp(V[left(r)], V[right(r)]) ? left(r) : right(r) };
            if (this->cmp(V[m], V[r])) {
                std::swap(V[r], V[m]);
                r = m;
            } else {
                break;
            }
        }
    }

    void build(Vector<T>&& V0) {
        auto n { V0.size() };
        this->V.resize(n + 1);
        std::move(std::begin(V0), std::end(V0), std::begin(V) + 1);
        for (Rank r { n / 2 }; r > 0; --r) {
            siftDown(r);
        }
    }

public:
    Heap() : AbstractPriorityQueue<T>() {
        V.resize(1);
    }
    Heap(Vector<T>&& V, const std::function<bool(const T&, const T&)>& cmp = std::less<T>()) : AbstractPriorityQueue<T>(cmp) {
        build(std::move(V));
    }

    size_t size() const override {
        return V.size() - 1;
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
        auto ret { std::move(V[1]) };
        V[1] = std::move(V[V.size() - 1]);
        V.pop_back();
        siftDown(1);
        return ret;
    }

    const T& top() const override {
        return V[1];
    }

    template <typename T1>
    friend std::ostream& operator<<(std::ostream& os, const Heap<T1>& t);

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Heap<T>& t) {
    os << "Heap(" << t.V << ")";
    return os;
}

}