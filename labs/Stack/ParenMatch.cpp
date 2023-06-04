import Framework;
import Vector;
import Stack;
import std;

using namespace dslab;
using namespace std;

class ParenMatch : public Algorithm<bool(const string&)> {
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
    bool operator()(const string& expr) {
        Stack<char> S;
        for (char c : expr) {
            if (char l { left(c) }; l == c) {
                S.push(c);
            } else if (l) {
                if (S.empty() || S.top() != l) return false;
                S.pop();
            }
        }
        return S.empty();
    }
    string type_name() const override {
        return "Paren Match (Stack)";
    }
};

// 一些验证正确性的样例
vector<pair<string, bool>> testCases {
    {"()()()()()()()"s, true},
    {"(((((())))))"s, true},
    {"(((((()))))))"s, false},
    {"((((((())))))"s, false},
    {"([{}])"s, true},
    {"([{]})"s, false}
};

// 使用自动生成得到的括号序列
vector testData { 10, 10'000, 1'000'000, 10'000'000 };

TestFramework<ParenMatch, ParenMatchStack> test;
RandomStackOperation<char, string> randop;

int main() {
    cout << "Case test:" << endl;
    for (const auto& [seq, ans] : testCases) {
        cout << format("  {:15} -> {:6}", seq, ans) << endl;
        test(seq);
    }
    cout << "Random test:" << endl;
    for (auto n : testData) {
        cout << format("Testing with {:>10} elements...", n) << endl;
        auto expr { randop(n, '(', ')') };
        test(expr);
    }
}