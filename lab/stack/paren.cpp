#include "stack.hpp"

using namespace dslab;
using namespace std::literals::string_literals;

class ParenMatch : public Algorithm<bool(const std::string&)> {
protected:
    char left(char c) {
        switch (c) {
            case '(': case ')': return '(';
            case '[': case ']': return '[';
            case '{': case '}': return '{';
            default: return 0;
        }
    }
};

class ParenMatchStack : public ParenMatch {
public:
    bool operator()(const std::string& expr) {
        Stack<char> S;
        for (auto c : expr) {
            if (auto l { left(c) }; l == c) {
                S.push(c);
            } else if (l) {
                if (S.empty() || S.top() != l) return false;
                S.pop();
            }
        }
        return S.empty();
    }
    std::string type_name() const override {
        return "Paren Match (Stack)";
    }
};

// some test cases for correctness
std::vector<std::pair<std::string, bool>> testCases {
    {"()()()()()()()"s, true},
    {"(((((())))))"s, true},
    {"(((((()))))))"s, false},
    {"((((((())))))"s, false},
    {"([{}])"s, true},
    {"([{]})"s, false}
};

// use automatically generated paren sequences
std::vector testData { 10, 10'000, 1'000'000, 10'000'000 };

TestFramework<ParenMatch, ParenMatchStack> test;
RandomStack<> randop;

int main() {
    std::cout << "Case test:" << std::endl;
    for (const auto& [seq, ans] : testCases) {
        std::cout << std::format("  {:15} -> {:6}", seq, ans) << std::endl;
        test(seq);
    }
    std::cout << "Random test:" << std::endl;
    for (auto n : testData) {
        std::cout << std::format("  {:10} elements: ", n);
        test(randop(n, '(', ')'));
    }

}

