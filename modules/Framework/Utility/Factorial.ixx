export module Factorial;

import Framework.Algorithm;
import std;

export namespace dslab {

class FactorialProblem : public Algorithm<int(int)> {};

class Factorial : public FactorialProblem {
    constexpr int factorial(int n) const {
        return n == 0 ? 1 : n * factorial(n - 1);
    }
public:
    int operator()(int n) override {
        return factorial(n);
    }
    std::string type_name() const override {
        return "Factorial (constexpr)";
    }
};

}