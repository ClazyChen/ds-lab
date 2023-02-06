#include <vector>
#include <format>
#include <iostream>
import Framework;
import Vector;
import BinaryTree;
import Stack;
using namespace dslab;
using namespace std;

template <typename T>
class Rebuild : public Algorithm<BinaryTree<T>, const Vector<T>&, const Vector<T>&> {};

template <typename T>
class RebuildBasic : public Rebuild<T> {
    using Iterator = typename Vector<T>::iterator;
    unique_ptr<BinaryTreeNode<T>> rebuild(Iterator preL, Iterator preR, Iterator inL, Iterator inR) {
        if (preL == preR) {
            return nullptr;
        }
        auto root { make_unique<BinaryTreeNode<T>>(*preL) };
        auto inRoot { find(inL, inR, *preL) };
        auto leftSize { distance(inL, inRoot) };
        root->left() = rebuild(next(preL), next(preL, leftSize + 1), inL, inRoot);
        root->right() = rebuild(next(preL, leftSize + 1), preR, next(inRoot), inR);
        return root;
    }
public:
    BinaryTree<T> operator()(const Vector<T>& pre, const Vector<T>& in) override {
        return BinaryTree<T>(rebuild(begin(pre), end(pre), begin(in), end(in)));
    }
};

template <typename T>
class RebuildLinear : public Rebuild<T> {
    using Iterator = typename Vector<T>::iterator;
    Iterator preEnd, inEnd, preCur, inCur;
    unique_ptr<BinaryTreeNode<T>> rebuild(Iterator parent) {
        if ((parent == preEnd && inCur == inEnd) || (*parent == *inCur)) {
            return nullptr;
        }
        auto leftL { preCur };
        while (*(preCur++) != *inCur) {}
        auto leftR { preCur };
        unique_ptr<BinaryTreeNode<T>> root { nullptr };
        for (--leftR; leftR != parent; ) {
            auto node { make_unique<BinaryTreeNode<T>>(*(inCur++)) };
            leftR = (leftR == leftL ? parent : --leftR);
            auto right { rebuild(leftR) };
            if (right) {
                right->parent() = node.get();
            }
            if (root) {
                root->parent() = node.get();
            }
            node->left() = move(root);
            node->right() = move(right);
            root = move(node);
        }
        return root;
    }
public:
    BinaryTree<T> operator()(const Vector<T>& pre, const Vector<T>& in) override {
        preCur = begin(pre);
        preEnd = end(pre);
        inCur = begin(in);
        inEnd = end(in);
        return BinaryTree<T>(rebuild(preEnd));
    }
};

int main() {
    BinaryTree<int> BT;
    BT.insertAsRoot(1);
    cout << BT.size() << endl;
    return 0;
}