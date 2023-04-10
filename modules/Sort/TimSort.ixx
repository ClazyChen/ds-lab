module;
#include <algorithm>
#include <functional>

export module Sort.TimSort;

import Vector;
import Sort.AbstractSort;

export namespace dslab {

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractVector<T>, Linear<T>>
class TimSort : public AbstractSort<T, Linear> {
    static constexpr size_t MIN_MERGE { 32 };
    using Iterator = typename Linear<T>::iterator;
    size_t getMinRun(size_t n) {
        size_t r { 0 };
        while (n >= MIN_MERGE) {
            r |= n & 1;
            n >>= 1;
        }
        return n + r;
    }
    void insertionSort(Iterator lo, Iterator hi, Iterator start) {
        for (auto i { start }; i != hi; ++i) {
            auto pos { std::upper_bound(lo, i, *i, this->cmp) };
            if (pos != i) {
                auto tmp { std::move(*i) };
                std::move_backward(pos, i, i + 1);
                *pos = std::move(tmp);
            }
        }
    }
    size_t findRun(Iterator lo, Iterator hi) {
        auto runHi { lo + 1 };
        if (runHi == hi) {
            return 1;
        }
        if (this->cmp(*runHi++, *lo)) {
            while (runHi != hi && this->cmp(*runHi, *(runHi - 1))) {
                ++runHi;
            }
            std::reverse(lo, runHi);
        } else {
            while (runHi != hi && !this->cmp(*runHi, *(runHi - 1))) {
                ++runHi;
            }
        }
        return runHi - lo;
    }
    class RunStack {
        struct Run {
            Iterator start;
            size_t len;
            bool operator==(const Run& rhs) const = default;
        };
        Vector<Run> runs;
        Vector<T> W;
        std::function<bool(const T&, const T&)> cmp;
        size_t size() const {
            return runs.size();
        }
        void mergeForward(Iterator lo, Iterator mi, Iterator hi) {
            W.resize(mi - lo);
            std::move(lo, mi, std::begin(W));
            auto i { std::begin(W) };
            auto j { mi }, k { lo };
            while (i != std::end(W) && j != hi) {
                if (cmp(*j, *i)) {
                    *k++ = std::move(*j++);
                } else {
                    *k++ = std::move(*i++);
                }
            }
            std::move(i, std::end(W), k);
        }
        void mergeBackward(Iterator lo, Iterator mi, Iterator hi) {
            W.resize(hi - mi);
            std::move(mi, hi, std::begin(W));
            auto i { std::end(W) }, j { mi }, k { hi };
            while (i != std::begin(W) && j != lo) {
                if (cmp(*(i-1), *(j-1))) {
                    *--k = std::move(*--j);
                } else {
                    *--k = std::move(*--i);
                }
            }
            std::move(std::begin(W), i, lo);
        }
        void merge(Iterator lo, Iterator mi, Iterator hi) {
            if (cmp(*(mi - 1), *mi)) {
                return;
            }
            if (mi - lo < hi - mi) {
                mergeForward(lo, mi, hi);
            } else {
                mergeBackward(lo, mi, hi);
            }
        }
    public:
        RunStack(std::function<bool(const T&, const T&)> cmp) : cmp { cmp } {}
        void push(Iterator lo, size_t runSize) {
            runs.push_back({ lo, runSize });
        }
        void maintainInvariant() {
            for (auto n { size() }; n > 1; --n) {
                auto& X { runs[n - 1] };
                auto& Y { runs[n - 2] };
                if (n > 2) {
                    if (auto& Z { runs[n - 3] }; Z.len <= Y.len + X.len) {
                        if (Z.len < X.len) {
                            merge(Z.start, Y.start, Y.start + Y.len);
                            Z.len += Y.len;
                            Y = X;
                        } else {
                            merge(Y.start, X.start, X.start + X.len);
                            Y.len += X.len;
                        }
                        runs.pop_back();
                        continue;
                    }
                }
                if (Y.len <= X.len) {
                    merge(Y.start, X.start, X.start + X.len);
                    Y.len += X.len;
                    runs.pop_back();
                } else {
                    break;
                }
            }
        }
        void mergeAll() {
            for (auto n { size() }; n > 1; --n) {
                auto& X { runs[n - 1] };
                auto& Y { runs[n - 2] };
                merge(Y.start, X.start, X.start + X.len);
                Y.len += X.len;
                runs.pop_back();
            }
        }
    };
    void timSort(Iterator lo, Iterator hi, size_t n) {
        if (n < 2) {
            return;
        } else if (n < MIN_MERGE) {
            insertionSort(lo, hi, lo + 1);
        } else {
            RunStack stack { this->cmp };
            size_t minRun { getMinRun(n) };
            do {
                auto runSize { findRun(lo, hi) };
                if (runSize < minRun) {
                    auto force { std::min(minRun, n) };
                    insertionSort(lo, lo + force, lo + runSize);
                    runSize = force;
                }
                stack.push(lo, runSize);
                stack.maintainInvariant();
                lo += runSize;
                n -= runSize;
            } while (n > 0);
            stack.mergeAll();
        }
    }
public:
    void sort(Linear<T>& V) override {
        timSort(std::begin(V), std::end(V), V.size());
    }
};

}
