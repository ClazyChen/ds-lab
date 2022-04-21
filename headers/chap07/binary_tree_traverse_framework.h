#pragma once

#include "binary_tree_framework.h"

// 这个文件讨论了一个经典的问题：
// 如何对二叉树进行遍历？
// 在相关的文件中，我引入了以下的算法：
// 1. 先序遍历（6种）
// 2. 中序遍历（5种）
// 3. 后序遍历（4种）
// 4. 层次遍历（1种）
// 具体每种遍历的设计思路，在相关的文件中有详细的说明

namespace clazy_framework {

template <typename T, typename Container>
requires (is_base_of_v<clazy_framework::AbstractBinaryTree<T>, Container>)
class BinaryTreeTraverse : public Algorithm<void, const Container&, const function<void(T&)>&> {
protected:
    virtual void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) = 0;
public:
    void apply(const Container& tree, const function<void(T&)>& visit) override {
        traverse(tree.root(), visit);
    }
};

}