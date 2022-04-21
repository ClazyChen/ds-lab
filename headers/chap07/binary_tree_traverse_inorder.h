#pragma once

#include "stack.h"
#include "binary_tree.h"
#include "binary_tree_traverse_framework.h"

// 这个文件展示了5种中序遍历的实现形式
// 1. 经典的中序遍历（LNR）
// 2. 和先序遍历对称的迭代形式（围绕最左侧通路的藤）
// 3. 和先序遍历对称的递归形式（围绕最左侧通路的藤）

// 4. 借助中序遍历迭代器
// 5. 借助中序线索二叉树

// 其中1、2、4为邓俊辉《数据结构》上的

namespace clazy {

// 经典做法：LNR递归（SLR）
template <typename T, typename Container = BinaryTree<T>>
class InOrderTraverse : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        if (pos != nullptr) {
            traverse(pos->left(), visit);
            visit(pos->data());
            traverse(pos->right(), visit);
        }
    }
};

// 基于藤缠树的迭代形式（围绕最左侧通路的藤）
template <typename T, typename Container = BinaryTree<T>>
class InOrderTraverseLI : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        Stack<BinaryTreeNodePos<T>> S;
        auto pushChain = [&S](BinaryTreeNodePos<T> pos) {
            while (pos != nullptr) {
                S.push(pos);
                pos = pos->left();
            }
        };
        while (pushChain(pos), !S.empty()) { // 每次入栈整个最左侧通路
            pos = S.pop();
            visit(pos->data());
            pos = pos->right();
        }
    }
};

// 基于藤缠树的递归形式（围绕最左侧通路的藤）
// 很容易发现，这个算法的实现形式和先序遍历的线性递归高度一致
// 二者的唯一区别在于visit函数的调用顺序
template <typename T, typename Container = BinaryTree<T>>
class InOrderTraverseLR : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        if (pos != nullptr) {
            while (pos->left() != nullptr) {
                pos = pos->left();
            }
            while (visit(pos->data()), traverse(pos->right(), visit), pos->isLeft()) {
                pos = pos->parent();
            }
        }
    }
};

// 使用迭代器得到的递归方法
// II - Iterator Iterative
template<typename T, typename Container = BinaryTree<T>>
class InOrderTraverseII : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    using IA = InOrderIterator<T>;
    using Iterator = clazy_framework::BinaryTreeIterator<T, IA>;

    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        for (auto it = Iterator(IA().first(pos)); it.getPosition() != nullptr; it++) {
            visit(*it);
        }
    }
};

}