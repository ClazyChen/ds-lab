﻿import Framework;
import Stack;
import Vector;
import Tree;
import std;

using namespace dslab;
using namespace std;

RandomStackOperation<char, string> randop;

class Stack2TreeProblem : public Algorithm<bool(string)> {};

class Stack2Tree : public Stack2TreeProblem {
    Tree<int> buildTree(string op) {
        Tree<int> T { 0 };
        auto pos { T.root() };
        int cur { 1 };
        for (char c : op) {
            if (c == '(') {
                pos = T.insertAsChild(pos, cur++);
            } else {
                pos = pos->parent();
            }
        }
        return T;
    }

    Vector<int> getPopSeq(string op) {
        Vector<int> V {};
        Stack<int> S {};
        int cur { 1 };
        for (char c : op) {
            if (c == '(') {
                S.push(cur++);
            } else {
                V.push_back(S.pop());
            }
        }
        return V;
    }

    bool check(const Tree<int>& T, const Vector<int>& V) {
        bool pass { true };
        int cur { 0 };
        T.traverse<TreePreOrderTraverse>([&pass, &cur](const int& e) {
            if (cur == e) {
                ++cur;
            } else {
                pass = false;
            }
        });
        size_t pos { 0 };
        T.traverse<TreePostOrderTraverse>([&pass, &pos, &V](const int& e) {
            if (pos < V.size()) {
                if (V[pos] == e) {
                    ++pos;
                } else {
                    pass = false;
                }
            }
        });
        return pass;
    }

public:
    bool operator()(string op) {
        auto T { buildTree(op) };
        auto V { getPopSeq(op) };
        return check(T, V);
    }

    string type_name() const override {
        return "Stack to Tree";
    }
};

vector testData { 10, 1000, 10000, 100'000 };

TestFramework<Stack2TreeProblem, Stack2Tree> test;

int main() {
    for (auto n : testData) {
        cout << format("n = {:>8}", n) << endl;
        test(randop(n, '(', ')'));
    }
}