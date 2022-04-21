#pragma once

#include "stack.h"
#include "binary_tree.h"
#include "binary_tree_traverse_framework.h"

// 这个文件展示了4种后序遍历的实现形式
// 1. 经典的后序遍历（LRN）
// 2. 借助藤缠树的迭代形式（通往HLVFL的藤）
// 3. 借助对称性的迭代形式（利用逆后序遍历和先序遍历的对称性）

// 4. 借助后序遍历迭代器

// 其中1、2为邓俊辉《数据结构》上的
// 相对来说，后序遍历如果用“正序”的观点看
// 则它是一个从下到上的遍历过程，但树作为半线性结构，它的自然生长方向是从上到下的
// 这个矛盾导致方法2这种实现非常复杂
// 通常，我们在处理后序遍历的时候采用“逆序”的观点看
// 即，考虑逆后序遍历（NRL），它在序列上和后序序列完全相反（reverse）
// 而在实现上，和先序遍历高度对称（left和right交换），这可以让我们引出方法3
// 当然，类似于先序遍历的线性迭代算法，后序遍历也可以经由逆后序遍历的线性迭代算法得到

namespace clazy {

// 经典做法：LRN递归
template <typename T, typename Container = BinaryTree<T>>
class PostOrderTraverse : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        if (pos != nullptr) {
            traverse(pos->left(), visit);
            traverse(pos->right(), visit);
            visit(pos->data());
        }
    }
};

// 利用HLVFL藤缠树的迭代形式
template <typename T, typename Container = BinaryTree<T>>
class PostOrderTraverseLI : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        Stack<BinaryTreeNodePos<T>> S;
        auto pushChain = [&S](BinaryTreeNodePos<T> pos) {
            while (pos != nullptr) {
                S.push(pos);
                if (pos->left() != nullptr) {
                    pos = pos->left();
                } else {
                    pos = pos->right();
                }
            }
        };
        pushChain(pos);
        while (!S.empty()) {
            pos = S.pop();
            visit(pos->data());
            if (pos->isLeft()) {
                pushChain(pos->parent()->right());
            }
        }
    }
};

// 利用逆后序遍历和先序遍历对称性的迭代形式
// 1. 进行逆后序遍历，访问函数为“入辅助栈”
// 2. 逆后序遍历结束，辅助栈里就是逆后序序列
// 3. 然后再依次出栈，就是后序序列
template <typename T, typename Container = BinaryTree<T>>
class PostOrderTraverseSLI : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        Stack<BinaryTreeNodePos<T>> S;
        Stack<BinaryTreeNodePos<T>> SA;
        S.push(pos);
        while (!S.empty()) {
            pos = S.pop();
            if (pos != nullptr) {
                SA.push(pos);
                S.push(pos->left());
                S.push(pos->right());
            }
        }
        while (!SA.empty()) {
            visit(SA.pop()->data());
        }
    }
};

// 使用迭代器得到的递归方法
// II - Iterator Iterative
template<typename T, typename Container = BinaryTree<T>>
class PostOrderTraverseII : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    using IA = PostOrderIterator<T>;
    using Iterator = clazy_framework::BinaryTreeIterator<T, IA>;

    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        for (auto it = Iterator(IA().first(pos)); it.getPosition() != nullptr; it++) {
            visit(*it);
        }
    }
};

}
