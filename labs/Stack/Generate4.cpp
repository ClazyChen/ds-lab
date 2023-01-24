#include <vector>
#include <format>
#include <ranges>
#include <iostream>
import Vector;
import Power;
import Stack;
import Framework;
using namespace dslab;
using namespace std;

class Generate4 : public Algorithm<size_t, size_t> {
protected:
    Vector<size_t> V;
public:
    void initialize() {
        V.clear();
    }
};

class Generate4Solver : public Algorithm<void, Vector<size_t>&, size_t> {
protected:
    size_t minn, maxn;
public:
    Generate4Solver() = default;
    Generate4Solver(size_t w) : minn(Power {}(10, w - 1)), maxn(Power {}(10, w) - 1) {}
};

class Generate4RecursiveSolver : public Generate4Solver {
public:
    Generate4RecursiveSolver(size_t w) : Generate4Solver(w) {}
    void operator()(Vector<size_t>& V, size_t n) override {
        if (minn <= n && n <= maxn) {
            V.push_back(n);
        } else {
            for (size_t i : {1, 2, 3, 4}) {
                (*this)(V, n * 10 + i);
            }
        }
    }
};

class Generate4IterativeSolver : public Generate4Solver {
public:
    Generate4IterativeSolver(size_t w) : Generate4Solver(w) {}
    void operator()(Vector<size_t>& V, size_t n) override {
        Stack<size_t> S;
        S.push(n);
        while (!S.empty()) {
            n = S.pop();
            if (minn <= n && n <= maxn) {
                V.push_back(n);
            } else {
                for (size_t i : {4, 3, 2, 1}) {
                    S.push(n * 10 + i);
                }
            }
        }
    }
};

class Generate4Pred : public Algorithm<bool, size_t> {
    size_t minn, maxn;
public:
    Generate4Pred(size_t w) : minn(Power {}(10, w - 1)), maxn(Power {}(10, w) - 1) {}
    bool operator()(size_t n) override {
        return minn <= n && n <= maxn;
    }
};

class Generate4Bound : public Algorithm<void, size_t> {
    Vector<size_t>& V;
public:
    Generate4Bound(Vector<size_t>& V) : V(V) {}
    void operator()(size_t n) override {
        V.push_back(n);
    }
};

class Generate4Next : public Algorithm<vector<tuple<size_t>>, size_t> {
public:
    vector<tuple<size_t>> operator()(size_t n) override {
        return { { n * 10 + 1 }, { n * 10 + 2 }, { n * 10 + 3 }, { n * 10 + 4 } };
    }
};

template <typename Pred, typename Bound, typename Next, typename... Args>
    requires is_invocable_r_v<bool, Pred, Args...>&& is_invocable_r_v<void, Bound, Args...>&& is_invocable_r_v<vector<tuple<Args...>>, Next, Args...>
class TailRecursive : public Algorithm<void, Args...> {
protected:
    unique_ptr<Pred> pred { nullptr };
    unique_ptr<Bound> bound { nullptr };
    unique_ptr<Next> next { nullptr };
public:
    TailRecursive(Pred&& pred, Bound&& bound, Next&& next) : pred { make_unique<Pred>(pred) }, bound { make_unique<Bound>(bound) }, next { make_unique<Next>(next) } {}
    void operator()(Args... args) override {
        if ((*pred)(args...)) {
            (*bound)(args...);
        } else {
            for (auto&& nextArgs : (*next)(args...)) {
                apply(*this, nextArgs);
            }
        }
    }
};

template <typename Pred, typename Bound, typename Next, typename... Args>
    requires is_invocable_r_v<bool, Pred, Args...>&& is_invocable_r_v<void, Bound, Args...>&& is_invocable_r_v<vector<tuple<Args...>>, Next, Args...>
class TailRecursiveIterator : public Algorithm<void, Args...> {
protected:
    unique_ptr<Pred> pred { nullptr };
    unique_ptr<Bound> bound { nullptr };
    unique_ptr<Next> next { nullptr };
public:
    TailRecursiveIterator(Pred&& pred, Bound&& bound, Next&& next) : pred { make_unique<Pred>(pred) }, bound { make_unique<Bound>(bound) }, next { make_unique<Next>(next) } {}
    void operator()(Args... args) override {
        Stack<tuple<Args...>> S;
        S.push({ args... });
        while (!S.empty()) {
            tie(args...) = S.pop();
            if ((*pred)(args...)) {
                (*bound)(args...);
            } else {
                for (auto&& nextArgs : (*next)(args...) | views::reverse) {
                    S.push(std::move(nextArgs));
                }
            }
        }
    }
};

template <typename Solver>
    requires is_base_of_v<Generate4Solver, Solver>
class Generate4Basic : public Generate4 {
public:
    size_t operator()(size_t w) override {
        Solver solver { w };
        solver(V, 0);
        return V.size();
    }
};

class Generate4RecursiveTemplate : public Generate4 {
public:
    size_t operator()(size_t w) override {
        TailRecursive<Generate4Pred, Generate4Bound, Generate4Next, size_t> solver { Generate4Pred { w }, Generate4Bound { V }, Generate4Next {} };
        solver(0);
        return V.size();
    }
    string type_name() const override {
        return "Generate4 Recursive (template)";
    }
};

class Generate4IterativeTemplate : public Generate4 {
public:
    size_t operator()(size_t w) override {
        TailRecursiveIterator<Generate4Pred, Generate4Bound, Generate4Next, size_t> solver { Generate4Pred { w }, Generate4Bound { V }, Generate4Next {} };
        solver(0);
        return V.size();
    }
    string type_name() const override {
        return "Generate4 Iterative (template)";
    }
};

vector testData { 4, 5, 6, 7, 8, 9 };

TestFramework<Generate4,
    Generate4Basic<Generate4RecursiveSolver>,
    Generate4Basic<Generate4IterativeSolver>,
    Generate4RecursiveTemplate,
    Generate4IterativeTemplate> test;

int main() {
    for (auto n : testData) {
        test.run(&Generate4::initialize);
        test(n);
    }
}