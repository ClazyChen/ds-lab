import Framework;
import Vector;
import BinaryTree;
import Stack;
import std;

using namespace dslab;
using namespace std;

template <typename T>
class Rebuild : public Algorithm<BinaryTree<T>(const Vector<T>&, const Vector<T>&)> {};

template <typename T>
class RebuildSeqLookup : public Rebuild<T> {
    BinaryTreeNodeInst<T> rebuild(const Vector<T>& pre, const Vector<T>& in, Rank preL, Rank preR, Rank inL, Rank inR) {
        if (preL == preR) {
            return nullptr;
        }
        auto root { BinaryTreeNodeInst<T>::make(pre[preL]) };
        auto inRoot { find(begin(in) + inL, begin(in) + inR, pre[preL]) - begin(in) };
        auto leftSize { inRoot - inL };
        if (auto left { rebuild(pre, in, preL + 1, preL + 1 + leftSize, inL, inRoot) }) {
            left->parent() = root;
            root->left() = move(left);
        }
        if (auto right { rebuild(pre, in, preL + 1 + leftSize, preR, inRoot + 1, inR) }) {
            right->parent() = root;
            root->right() = move(right);
        }
        return root;
    }

public:
    BinaryTree<T> operator()(const Vector<T>& pre, const Vector<T>& in) override {
        return BinaryTree<T>(rebuild(pre, in, 0, pre.size(), 0, in.size()));
    }

    string type_name() const override {
        return "Rebuild (Sequential Lookup)";
    }
};

template <typename T, bool UseVector = false>
class RebuildTable : public Rebuild<T> {
    Vector<Rank> pre2in;
    void buildTable(const Vector<T>& pre, const Vector<T>& in) {
        if constexpr (UseVector) {
            Vector<Rank> table;
            pre2in.resize(pre.size());
            table.resize(in.size());
            for (Rank r { 0 }; r < in.size(); ++r) {
                table[in[r]] = r;
            }
            for (Rank r { 0 }; r < pre.size(); ++r) {
                pre2in[r] = table[pre[r]];
            }
        } else {
            unordered_map<T, Rank> table;
            pre2in.resize(pre.size());
            for (Rank r { 0 }; r < in.size(); ++r) {
                table.emplace(in[r], r);
            }
            for (Rank r { 0 }; r < pre.size(); ++r) {
                pre2in[r] = table[pre[r]];
            }
        }
    }

    BinaryTreeNodeInst<T> rebuild(const Vector<T>& pre, const Vector<T>& in, Rank preL, Rank preR, Rank inL, Rank inR) {
        if (preL == preR) {
            return nullptr;
        }
        auto root { BinaryTreeNodeInst<T>::make(pre[preL])};
        auto inRoot { pre2in[preL] };
        auto leftSize { inRoot - inL };
        if (auto left { rebuild(pre, in, preL + 1, preL + 1 + leftSize, inL, inRoot) }) {
            left->parent() = root;
            root->left() = move(left);
        }
        if (auto right { rebuild(pre, in, preL + 1 + leftSize, preR, inRoot + 1, inR) }) {
            right->parent() = root;
            root->right() = move(right);
        }
        return root;
    }

public:
    BinaryTree<T> operator()(const Vector<T>& pre, const Vector<T>& in) override {
        buildTable(pre, in);
        return BinaryTree<T>(rebuild(pre, in, 0, pre.size(), 0, in.size()));
    }

    string type_name() const override {
        if constexpr (UseVector) {
            return "Rebuild (Reverse Table / Vector)";
        } else {
            return "Rebuild (Reverse Table / std::unordered_map)";
        }
    }

};

template <typename T>
class RebuildLinear : public Rebuild<T> {
    Rank preL, inL;
    
    BinaryTreeNodePos<T> buildLeftMost(const Vector<T>& pre, const Vector<T>& in, BinaryTreeNodePos<T> start) {
        auto pos { start };
        while (pre[preL] != in[inL]) {
            auto node { BinaryTreeNodeInst<T>::make(pre[++preL]) };
            node->parent() = pos;
            pos = pos->left() = move(node);
        }
        return pos;
    }

    BinaryTreeNodeInst<T> rebuild(const Vector<T>& pre, const Vector<T>& in, optional<T> end) {
        if (inL == in.size() || (end.has_value() && in[inL] == end)) {
            return nullptr;
        }
        auto root { BinaryTreeNodeInst<T>::make(pre[preL]) };
        auto pos { buildLeftMost(pre, in, root) };
        ++preL;
        while (pos) {
            ++inL;
            if (auto right { rebuild(pre, in, pos == root ? end : pos->parent()->data()) }; right) {
                right->parent() = pos;
                pos->right() = move(right);
            }
            pos = pos->parent();
        }
        return root;
    }

public:
    BinaryTree<T> operator()(const Vector<T>& pre, const Vector<T>& in) override {
        preL = inL = 0;
        return BinaryTree<T>(rebuild(pre, in, optional<T>{}));
    }

    string type_name() const override {
        return "Rebuild (Linear Recursion)";
    }
};

// 生成测试用例的方法：随机生成一棵二叉树，然后前序遍历和中序遍历，再用这两个序列重建二叉树
// 在这里演示的是利用栈和长女-次妹表示法生成二叉树的技术
class Stack2BinaryTree : public Algorithm<BinaryTree<int>(size_t)> {
    BinaryTree<int> buildTree(string op) {
        BinaryTree<int> T {};
        auto pos { T.root() };
        int cur { 0 };
        bool left { true };
        for (char c : op) {
            if (c == '(') {
                if (T.empty()) {
                    pos = T.insertAsRoot(cur++);
                } else {
                    if (left) {
                        pos = T.insertAsLeftChild(pos, cur++);
                    } else {
                        pos = T.insertAsRightChild(pos, cur++);
                    }
                }
                left = true;
            } else {
                if (left) {
                    left = false;
                } else {
                    while (pos->isRightChild()) {
                        pos = pos->parent();
                    }
                    pos = pos->parent();
                }
            }
        }
        return T;
    }

    RandomStackOperation<char, string> randop;
public:
    BinaryTree<int> operator()(size_t n) override {
        return buildTree(randop(n, '(', ')'));
    }
};

template <typename T>
class RebuildBinaryTreeTest : public Algorithm<void(const Vector<T>&, const Vector<T>&)> {
public:
    virtual void check(const Vector<T>& pre, const Vector<T>& in) = 0;
};

template <typename T, template <typename> typename RebuildAlgorithm>
requires is_base_of_v<Rebuild<T>, RebuildAlgorithm<T>>
class RebuildBinaryTreeTestImpl : public RebuildBinaryTreeTest<T> {
    BinaryTree<T> tree;
    RebuildAlgorithm<T> rebuild;

public:
    void operator()(const Vector<T>& pre, const Vector<T>& in) override {
        tree = rebuild(pre, in);
    }

    void check(const Vector<T>& pre, const Vector<T>& in) override {
        bool pass { true };
        Rank pos { 0 };
        tree.traverse<BinaryTreePreOrderTraverse>([&pos, &pre, &pass](auto&& e) {
            if (e != pre[pos++]) {
                pass = false;
            }
        });
        pos = 0;
        tree.traverse<BinaryTreeInOrderTraverse>([&pos, &in, &pass](auto&& e) {
            if (e != in[pos++]) {
                pass = false;
            }
        });
        if (!pass) {
            cout << format("ERROR: {} failed!", type_name()) << endl;
        }
    }

    string type_name() const override {
        return rebuild.type_name();
    }
};

vector testData { 10, 100, 1000, 3000, 10000, 30000, 100'000 };
Stack2BinaryTree buildTree;

template <typename T>
using RebuildTableVecVer = RebuildTable<T, true>;

TestFramework<RebuildBinaryTreeTest<int>,
    RebuildBinaryTreeTestImpl<int, RebuildSeqLookup>,
    RebuildBinaryTreeTestImpl<int, RebuildTable>,
    RebuildBinaryTreeTestImpl<int, RebuildTableVecVer>,
    RebuildBinaryTreeTestImpl<int, RebuildLinear>
> test;

int main() {
    for (auto n : testData) {
        cout << format("n = {:>8}", n) << endl;
        auto tree { buildTree(n) };
        Vector<int> pre, in;
        tree.traverse<BinaryTreePreOrderTraverse>([&pre](auto&& e) { pre.push_back(e); });
        tree.traverse<BinaryTreeInOrderTraverse>([&in](auto&& e) { in.push_back(e); });
        test(cref(pre), cref(in));
        test.run([&pre, &in](auto&& test) { test.check(pre, in); });
    }
    return 0;
}