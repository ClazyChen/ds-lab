export module Power;

import Framework.Algorithm;
export namespace dslab {

class PowerProblem : public Algorithm<int, int, int> {};

class Power : public PowerProblem {
public:
    int operator()(int a, int b) override {
        int result { 1 };
        while (b > 0) {
            if (b % 2 == 1) {
                result *= a;
            }
            a *= a;
            b /= 2;
        }
        return result;
    }
};

}