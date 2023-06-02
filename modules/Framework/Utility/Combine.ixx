export module Combine;

import Framework.Algorithm;
import std.compat;

export namespace dslab {

class CombineProblem : public Algorithm<int(int, int)> {};

class Combine : public CombineProblem {
    std::vector<std::vector<int>> C;
public:
    int operator()(int n, int m) override {
        m = std::min(m, n - m);
        if (n < C.size() && m < C[n].size())
            return C[n][m];
        while (n >= C.size()) {
            C.push_back(std::vector<int> { 1 });
        }
        for (int i = C[n].size(); i <= m; ++i) {
            C[n].push_back(static_cast<long long>(n - i + 1) * C[n].back() / i);
        }
        return C[n][m];
    }

    std::string type_name() const override {
        return "Combine (1-D Recursive)";
    }
};

}