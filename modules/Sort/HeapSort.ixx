export module Sort.HeapSort;

import Vector;
import Sort.AbstractSort;
import PriorityQueue.Heap;
import std;

export namespace dslab {

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractVector<T>, Linear<T>>
class HeapSort : public AbstractSort<T, Linear> {
    constexpr static Rank left(Rank r) {
        return r * 2 + 1;
    }

    constexpr static Rank right(Rank r) {
        return r * 2 + 2;
    }

    void siftDown(Linear<T>& V, Rank n, Rank r) {
        while (true) {
            auto m { r }, lc { left(r) }, rc { right(r) };
            if (lc < n && this->cmp(V[m], V[lc])) {
                m = lc;
            }
            if (rc < n && this->cmp(V[m], V[rc])) {
                m = rc;
            }
            if (m == r) {
                break;
            } else {
                std::swap(V[r], V[m]);
                r = m;
            }
        }
    }

protected:
    void sort(Linear<T>& V) override {
        if (int n = V.size(); n > 0) {
            for (auto r { n / 2 }; r >= 0; --r) {
                siftDown(V, n, r);
            }
            for (auto r { n - 1 }; r >= 0; --r) {
                std::swap(V[0], V[r]);
                siftDown(V, r, 0);
            }
        }
    }
public:
    std::string type_name() const override {
        return "Heap Sort";
    }
};

}