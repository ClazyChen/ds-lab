export module Tree.RandomTree;

import Tree;
import Framework.Algorithm;
import Stack.RandomStackOperation;
import std;

export namespace dslab {

template <typename T>
class RandomTree : public Algorithm<Tree<T>(size_t)> {
    RandomStackOperation<char, std::string> randop {};

public:
    Tree<T> operator()(size_t n) override {
        if (n == 0) {
            return Tree<T> {};
        } 
        Tree<T> tree { T {} };
        auto pos { T.root() };
        auto vec { randop(n-1, '(', ')') };
        for (auto c : vec) {
            if (c == '(') {
                pos = tree.insertAsChild(pos, T {});
            } else {
                pos = pos->parent();
            }
        }
        return tree;
    }
};

}