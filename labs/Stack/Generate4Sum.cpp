#include <vector>
#include <numeric>
#include <optional>
#include <format>
#include <iostream>
import Vector;
import Power;
import Stack;
import Framework;
using namespace dslab;
using namespace std;

class Generate4Sum : public Algorithm<size_t(size_t)> {};

class Generate4SumSolver : public Algorithm<size_t(size_t)> {
protected:
    size_t minn, maxn;
public:
    Generate4SumSolver() = default;
    Generate4SumSolver(size_t w) : minn(Power {}(10, w - 1)), maxn(Power {}(10, w) - 1) {}
};

class Generate4SumRecursiveSolver : public Generate4SumSolver {
public:
    Generate4SumRecursiveSolver(size_t w) : Generate4SumSolver(w) {}
    size_t operator()(size_t n) override {
        if (minn <= n && n <= maxn) {
            return n;
        } else {
            return (*this)(n * 10 + 1) + (*this)(n * 10 + 2) + (*this)(n * 10 + 3) + (*this)(n * 10 + 4);
        }
    }
};

class Generate4SumIterativeSolver : public Generate4SumSolver {
public:
    Generate4SumIterativeSolver(size_t w) : Generate4SumSolver(w) {}
    size_t operator()(size_t n) override {
        Stack<size_t> S { n };
        size_t sum = 0;
        while (!S.empty()) {
            n = S.pop();
            if (minn <= n && n <= maxn) {
                sum += n;
            } else {
                for (size_t i : {4, 3, 2, 1}) {
                    S.push(n * 10 + i);
                }
            }
        }
        return sum;
    }
};

class Generate4Pred : public Algorithm<bool(size_t)> {
    size_t minn, maxn;
public:
    Generate4Pred(size_t w) : minn(Power {}(10, w - 1)), maxn(Power {}(10, w) - 1) {}
    bool operator()(size_t n) override {
        return minn <= n && n <= maxn;
    }
};

class Generate4Bound : public Algorithm<size_t(size_t)> {
public:
    size_t operator()(size_t n) override {
        return n;
    }
};

class Generate4HasNext : public Algorithm<bool(vector<size_t>&, size_t)> {
public:
    bool operator()(vector<size_t>& V, size_t n) override {
        return V.size() < 4;
    }
};

class Generate4Next : public Algorithm<tuple<size_t>(vector<size_t>&, size_t)> {
public:
    tuple<size_t> operator()(vector<size_t>& V, size_t n) override {
        return { n * 10 + V.size() + 1 };
    }
};

class Generate4Finalize : public Algorithm<size_t(vector<size_t>&, size_t)> {
public:
    size_t operator()(vector<size_t>& V, size_t n) override {
        return V[0] + V[1] + V[2] + V[3];
    }
};

template <typename R, typename Pred, typename Bound, typename HasNext, typename Next, typename Finalize, typename... Args>
    requires is_invocable_r_v<bool, Pred, Args...>
&& is_invocable_r_v<R, Bound, Args...>
&& is_invocable_r_v<bool, HasNext, vector<R>&, Args...>
&& is_invocable_r_v<tuple<Args...>, Next, vector<R>&, Args...>
&& is_invocable_r_v<R, Finalize, vector<R>&, Args...>
class Recursive : public Algorithm<R(Args...)> {
protected:
    unique_ptr<Pred> pred { nullptr };
    unique_ptr<Bound> bound { nullptr };
    unique_ptr<HasNext> hasnext { nullptr };
    unique_ptr<Next> next { nullptr };
    unique_ptr<Finalize> finalize { nullptr };
public:
    Recursive(Pred&& pred, Bound&& bound, HasNext&& hasnext, Next&& next, Finalize&& finalize) :
        pred { make_unique<Pred>(pred) },
        bound { make_unique<Bound>(bound) },
        hasnext { make_unique<HasNext>(hasnext) },
        next { make_unique<Next>(next) },
        finalize { make_unique<Finalize>(finalize) } {
    }
    R operator()(Args... args) override {
        if ((*pred)(args...)) {
            return (*bound)(args...);
        } else {
            vector<R> V {};
            while ((*hasnext)(V, args...)) {
                V.push_back(apply(*this, (*next)(V, args...)));
            }
            return (*finalize)(V, args...);
        }
    }
};

template <typename R, typename Pred, typename Bound, typename HasNext, typename Next, typename Finalize, typename... Args>
    requires is_invocable_r_v<bool, Pred, Args...>
&& is_invocable_r_v<R, Bound, Args...>
&& is_invocable_r_v<bool, HasNext, vector<R>&, Args...>
&& is_invocable_r_v<tuple<Args...>, Next, vector<R>&, Args...>
&& is_invocable_r_v<R, Finalize, vector<R>&, Args...>
class Iterative : public Algorithm<R(Args...)> {
protected:
    unique_ptr<Pred> pred { nullptr };
    unique_ptr<Bound> bound { nullptr };
    unique_ptr<HasNext> hasnext { nullptr };
    unique_ptr<Next> next { nullptr };
    unique_ptr<Finalize> finalize { nullptr };
public:
    Iterative(Pred&& pred, Bound&& bound, HasNext&& hasnext, Next&& next, Finalize&& finalize) :
        pred { make_unique<Pred>(pred) },
        bound { make_unique<Bound>(bound) },
        hasnext { make_unique<HasNext>(hasnext) },
        next { make_unique<Next>(next) },
        finalize { make_unique<Finalize>(finalize) } {
    }
    R operator()(Args... args) override {
        vector<R> V {};
        Stack<tuple<vector<R>, Args...>> S { { {}, args... }, { {}, args... } };
        while (S.size() > 1) {
            tie(V, args...) = S.top();
            optional<R> r {};
            if ((*pred)(args...)) {
                r = (*bound)(args...);
            } else if ((*hasnext)(V, args...)) {
                tie(args...) = (*next)(V, args...);
                S.push({ {}, args... });
            } else {
                r = (*finalize)(V, args...);
            }
            if (r.has_value()) {
                S.pop();
                get<0>(S.top()).push_back(r.value());
            }
        }
        return get<0>(S.top())[0];
    }
};

class Generate4SumRecursive : public Generate4Sum {
public:
    size_t operator()(size_t w) override {
        return Recursive<size_t, Generate4Pred, Generate4Bound, Generate4HasNext, Generate4Next, Generate4Finalize, size_t> {
            Generate4Pred { w },
                Generate4Bound {},
                Generate4HasNext {},
                Generate4Next {},
                Generate4Finalize {}
        }(0);
    }
    string type_name() const override {
        return "Generate 4 Sum Recursive (template)";
    }
};

class Generate4SumIterative : public Generate4Sum {
public:
    size_t operator()(size_t w) override {
        return Iterative<size_t, Generate4Pred, Generate4Bound, Generate4HasNext, Generate4Next, Generate4Finalize, size_t> {
            Generate4Pred { w },
                Generate4Bound {},
                Generate4HasNext {},
                Generate4Next {},
                Generate4Finalize {}
        }(0);
    }
    string type_name() const override {
        return "Generate 4 Sum Iterative (template)";
    }
};

template <typename Solver>
    requires is_base_of_v<Generate4SumSolver, Solver>
class Generate4SumBasic : public Generate4Sum {
public:
    size_t operator()(size_t w) override {
        return Solver { w }(0);
    }
};

vector testData { 4, 5, 6, 7, 8 };

TestFramework<Generate4Sum, 
    Generate4SumBasic<Generate4SumRecursiveSolver>,
    Generate4SumBasic<Generate4SumIterativeSolver>,
    Generate4SumRecursive, Generate4SumIterative> test;

int main() {
    for (auto n : testData) {
        cout << format("n = {}", n) << endl;
        test(n);
    }
}