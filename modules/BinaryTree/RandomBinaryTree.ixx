export module BinaryTree.RandomBinaryTree;

import BinaryTree;
import Framework.Algorithm;
import Vector;
import std;

export namespace dslab {

template <typename T>
class RandomBinaryTree : public Algorithm<BinaryTree<T>(size_t)> {
    std::default_random_engine m_engine { std::random_device {}() };
    std::uniform_int_distribution<size_t> m_distribution { 0, std::numeric_limits<size_t>::max() };

    size_t rand(size_t n) {
        return m_distribution(m_engine) % n;
    }
public:
    BinaryTree<T> operator()(size_t n) override {
        auto root { BinaryTreeNodeInst<T>::make() };
        Vector<BinaryTreeNodePos<T>> nodes;
        Vector<bool> isLeaf;
        nodes.resize(2 * n - 1);
        isLeaf.resize(2 * n - 1);
        nodes[0] = root;
        isLeaf[0] = true;
        for (Rank r { 0 }; r < n; ++r) {
            auto zr { rand(2 * r - 1) };
            auto& z { nodes[zr] };
            auto x { BinaryTreeNodeInst<T>::make() };
            auto y { BinaryTreeNodeInst<T>::make() };
            y->left() = std::move(z->left());
            y->right() = std::move(z->right());
            nodes[2 * r - 1] = x;
            isLeaf[2 * r - 1] = true;
            nodes[2 * r] = y;
            isLeaf[2 * r] = isLeaf[zr];
            isLeaf[zr] = false;
            if (rand(2) == 0) {
                std::swap(x, y);
            }
            z->left() = std::move(y);
            z->right() = std::move(x);
        }
        for (Rank r { 0 }; r < 2 * n - 1; ++r) {
            if (!isLeaf[r]) {
                auto node { nodes[r - 1] };
                if (node->left()->isLeaf()) {
                    node->left() = nullptr;
                }
                if (node->right()->isLeaf()) {
                    node->right() = nullptr;
                }
            }
        }
        return BinaryTree<T> { std::move(root) };
    }
};

}