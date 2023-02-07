#include <vector>
#include <format>
#include <iostream>
import Framework;
import Search;
import Vector;
using namespace dslab;
using namespace std;

template <typename T, typename Comparator = std::less<T>>
class BinarySearchRecursive : public AbstractSearch<T, DefaultVector, Comparator> {
    Comparator cmp;
    Rank search(const Vector<T>& V, const T& e, Rank lo, Rank hi) const {
        if (hi - lo <= 1) {
            return lo < V.size() && cmp(V[lo], e) ? hi : lo;
        }
        Rank mi { lo + (hi - lo) / 2 };
        if (cmp(e, V[mi])) {
            return search(V, e, lo, mi);
        } else {
            return search(V, e, mi, hi);
        }
    }
public:
    Rank operator()(const Vector<T>& V, const T& e) override {
        return search(V, e, 0, V.size());
    }
};

template <typename T, typename Comparator = std::less<T>>
class BinarySearchIterative : public AbstractSearch<T, DefaultVector, Comparator> {
    Comparator cmp;
public:
    Rank operator()(const Vector<T>& V, const T& e) override {
        Rank lo { 0 }, hi { V.size() };
        while (hi - lo > 1) {
            Rank mi { lo + (hi - lo) / 2 };
            if (cmp(e, V[mi])) {
                hi = mi;
            } else {
                lo = mi;
            }
        }
        return lo < V.size() && cmp(V[lo], e) ? hi : lo;
    }
};

template <typename T>
class SearchPred : public Algorithm<bool(Rank, Rank)> {
public:
    bool operator()(Rank lo, Rank hi) override {
        return hi - lo <= 1;
    }
};

template <typename T, typename Comparator = std::less<T>>
class SearchBound : public Algorithm<Rank(Rank, Rank)> {
    Comparator cmp;
    const Vector<T>& V;
    const T& e;
public:
    SearchBound(const Vector<T>& V, const T& e) : V { V }, e { e } {}
    Rank operator()(Rank lo, Rank hi) override {
        return lo < V.size() && cmp(V[lo], e) ? hi : lo;
    }
};

template <typename T, typename Comparator = std::less<T>>
class SearchNext : public Algorithm<std::tuple<Rank, Rank>(Rank, Rank)> {
    Comparator cmp;
    const Vector<T>& V;
    const T& e;
public:
    SearchNext(const Vector<T>& V, const T& e) : V { V }, e { e } {}
    tuple<Rank, Rank> operator()(Rank lo, Rank hi) override {
        Rank mi { lo + (hi - lo) / 2 };
        if (cmp(e, V[mi])) {
            return { lo, mi };
        } else {
            return { mi, hi };
        }
    }
};

template <typename R, typename Pred, typename Bound, typename Next, typename... Args>
    requires is_invocable_r_v<bool, Pred, Args...>&& is_invocable_r_v<R, Bound, Args...>&& is_invocable_r_v<tuple<Args...>, Next, Args...>
class TailRecursive : public Algorithm<R(Args...)> {
protected:
    unique_ptr<Pred> pred { nullptr };
    unique_ptr<Bound> bound { nullptr };
    unique_ptr<Next> next { nullptr };
public:
    TailRecursive(Pred&& pred, Bound&& bound, Next&& next) : pred { make_unique<Pred>(pred) }, bound { make_unique<Bound>(bound) }, next { make_unique<Next>(next) } {}
    R operator()(Args... args) override {
        if ((*pred)(args...)) {
            return (*bound)(args...);
        }
        return apply(*this, (*next)(args...));
    }
};

template <typename R, typename Pred, typename Bound, typename Next, typename... Args>
    requires is_invocable_r_v<bool, Pred, Args...>&& is_invocable_r_v<R, Bound, Args...>&& is_invocable_r_v<tuple<Args...>, Next, Args...>
class TailRecursiveIterator : public Algorithm<R(Args...)> {
protected:
    unique_ptr<Pred> pred { nullptr };
    unique_ptr<Bound> bound { nullptr };
    unique_ptr<Next> next { nullptr };
public:
    TailRecursiveIterator(Pred&& pred, Bound&& bound, Next&& next) : pred { make_unique<Pred>(pred) }, bound { make_unique<Bound>(bound) }, next { make_unique<Next>(next) } {}
    R operator()(Args... args) override {
        while (!(*pred)(args...)) {
            tie(args...) = (*next)(args...);
        }
        return (*bound)(args...);
    }
};

template <typename T, typename Comparator = std::less<T>>
class BinarySearchRecursiveTemplate : public AbstractSearch<T, DefaultVector, Comparator> {
    unique_ptr<TailRecursive<Rank, SearchPred<T>, SearchBound<T, Comparator>, SearchNext<T, Comparator>, Rank, Rank>> search { nullptr };
public:
    Rank operator()(const Vector<T>& V, const T& e) override {
        search = make_unique<TailRecursive<Rank, SearchPred<T>, SearchBound<T, Comparator>, SearchNext<T, Comparator>, Rank, Rank>>(SearchPred<T>(), SearchBound<T, Comparator>(V, e), SearchNext<T, Comparator>(V, e));
        return (*search)(0, V.size());
    }
    string type_name() const override {
        return "BinarySearchRecursive(Template)";
    }
};

template <typename T, typename Comparator = std::less<T>>
class BinarySearchIterativeTemplate : public AbstractSearch<T, DefaultVector, Comparator> {
    unique_ptr<TailRecursiveIterator<Rank, SearchPred<T>, SearchBound<T, Comparator>, SearchNext<T, Comparator>, Rank, Rank>> search { nullptr };
public:
    Rank operator()(const Vector<T>& V, const T& e) override {
        search = make_unique<TailRecursiveIterator<Rank, SearchPred<T>, SearchBound<T, Comparator>, SearchNext<T, Comparator>, Rank, Rank>>(SearchPred<T>(), SearchBound<T, Comparator>(V, e), SearchNext<T, Comparator>(V, e));
        return (*search)(0, V.size());
    }
    string type_name() const override {
        return "BinarySearchIterative(Template)";
    }
};

class BinarySearchTestProblem : public Algorithm<Rank(const Vector<int>&, const int&)> {
public:
    void initialize(Vector<int>& V, size_t n, int lo, int hi) {
        V.resize(n);
        for (size_t i { 0 }; i < n; ++i) {
            V[i] = rand() % (hi - lo) + lo;
        }
        sort(V.begin(), V.end());
    }
};

template <typename Search>
    requires is_base_of_v<AbstractSearch<int>, Search>
class BinarySearchTest : public BinarySearchTestProblem {
    Search search;
    static constexpr size_t TIMES { 10000 };
public:
    Rank operator()(const Vector<int>& V, const int& e) override {
        Rank r { search(V, e) };
        for (size_t i { 0 }; i < TIMES; ++i) {
            if (r != search(V, e)) {
                throw runtime_error("BinarySearchTest: " + search.type_name() + " failed");
            }
        }
        return r;
    }
    string type_name() const override {
        return search.type_name();
    }
};

vector testData {
    10, 100, 1000, 10000, 100'000, 1'000'000
};

constexpr int lo { 0 }, hi { 5 };

TestFramework<BinarySearchTestProblem,
    BinarySearchTest<BinarySearchRecursive<int>>,
    BinarySearchTest<BinarySearchIterative<int>>,
    BinarySearchTest<BinarySearchRecursiveTemplate<int>>,
    BinarySearchTest<BinarySearchIterativeTemplate<int>>
> test;

int main() {
    for (Vector<int> V; auto n : testData) {
        cout << format("n = {}", n) << endl;
        test.run([&V, n](BinarySearchTestProblem& problem) {
            problem.initialize(ref(V), n, lo, hi);
            });
        test(cref(V), lo + (hi - lo) / 2);
    }
    return 0;
}


