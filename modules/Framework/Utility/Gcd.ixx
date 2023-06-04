export module Gcd;

import Framework.Algorithm;
import std;

export namespace dslab {

class GcdProblem : public Algorithm<int(int, int)> {};

class Gcd : public GcdProblem {
public:
    int operator()(int a, int b) override {
        while (b != 0) {
            int r { a % b };
            a = b;
            b = r;
        }
        return a;
    }
    std::string type_name() const override {
        return "Euclidean Gcd (Iterative)";
    }
};
}