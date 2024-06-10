#include "stack.hpp"

using namespace dslab;

using ParenOut = Algorithm<std::size_t(std::size_t)>;

class ParenOutRecursive : public ParenOut {
protected:
    Vector<std::string> ans;
    void search(std::string str, int l, int r) {
        if (l < 0 || l > r) return;
        if (r == 0) {
            ans.push_back(str);
        }
        search(str + '(', l - 1, r);
        search(str + ')', l, r - 1);
    }

public:
    std::size_t operator()(std::size_t n) {
        ans.clear();
        search("", n, n);
        return ans.size();
    }
    std::string type_name() const override {
        return "Paren Out (Recursive)";
    }

};

class ParenOutStack : public ParenOut {
protected:
    Vector<std::string> ans;
    void search(std::size_t n) {
        Stack<std::pair<std::string, std::pair<int, int>>> S;
        S.push({"", {n, n}});
        while (!S.empty()) {
            auto [str, p] = S.pop();
            auto [l, r] = p;
            if (l < 0 || l > r) continue;
            if (l == 0 && r == 0) {
                ans.push_back(str);
            }
            S.push({str + ')', {l, r - 1}});
            S.push({str + '(', {l - 1, r}});
        }
    }

public:
    std::size_t operator()(std::size_t n) {
        ans.clear();
        search(n);
        return ans.size();
    }
    std::string type_name() const override {
        return "Paren Out (Stack)";
    }
};

std::vector testData { 5, 10, 15 };

TestFramework<ParenOut, ParenOutRecursive, ParenOutStack> test;

int main() {
    for (auto& n : testData) {
        std::cout << std::format("n = {} : ", n)  << std::endl;
        test(n);
    }
    return 0;
}