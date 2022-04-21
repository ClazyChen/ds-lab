#pragma once

#include "stack.h"
#include "binary_tree.h"
#include "binary_tree_traverse_framework.h"

// 这个文件展示了6种先序遍历的实现模式
// 1. 经典的先序遍历（NLR）
// 2. NLR对应的迭代形式
// 3. 和中序遍历对称的迭代形式（围绕最左侧通路的藤）
// 4. 上述迭代形式对应的递归形式（线性递归）

// 5. 借助先序遍历迭代器
// 6. 借助先序线索二叉树

// 其中1、2、3为邓俊辉《数据结构》上的
// 为什么要引入实现模式4？
// 先序、中序、后序遍历的经典递归形式都是半线性递归
// 对于中序、后序遍历来说，这种递归模式不是尾递归，没法直接改写成迭代形式
// 为了让其变成迭代形式，中序、后序遍历都采用了藤缠树的思路，进行线性递归
// 在邓俊辉《数据结构》中揭示了这种藤缠树的思路，但没有介绍线性递归
// 理解线性递归的思想之后，能进一步提升对三种遍历的理解
// 比如，您可以很自然地理解，在构造（入栈序列，出栈序列）到二叉树（先序序列，中序序列）的一一映射的时候
// 为什么要围绕最左侧通路：因为最左侧通路事实上是先序遍历和中序遍历的一个对称点

namespace clazy {

// 经典做法：NLR递归（SLR）
template<typename T, typename Container = BinaryTree<T>>
class PreOrderTraverse : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        if (pos != nullptr) {
            visit(pos->data());
            traverse(pos->left(), visit);
            traverse(pos->right(), visit);
        }
    }
};

// 使用在《栈》一章中介绍的尾递归改写方法得到的迭代方法
// SLI - Semilinear Iterative
template<typename T, typename Container = BinaryTree<T>>
class PreOrderTraverseSLI : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        Stack<BinaryTreeNodePos<T>> S;
        S.push(pos);
        while (!S.empty()) {
            pos = S.pop();
            if (pos != nullptr) {
                visit(pos->data());
                S.push(pos->right());
                S.push(pos->left());
            }
        }
    }
};

// 使用藤缠树的思路得到的迭代方法
// LI - Linear Iterative
template<typename T, typename Container = BinaryTree<T>>
class PreOrderTraverseLI : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        Stack<BinaryTreeNodePos<T>> S;
        S.push(pos);
        while (!S.empty()) {
            for (pos = S.pop(); pos != nullptr; pos = pos->left()) {
                visit(pos->data());
                S.push(pos->right());
            }
        }
    }
};

// 使用藤缠树的思路得到的递归方法
// LR - Linear Recursive
// 这种方法在邓俊辉《数据结构》中没有提及
// 但您可以很容易理解，把上面的方法改写成递归形式就可以得到一个和NLR迥异的线性递归版本
// 1. 首先沿着左下方向，把所有沿途节点进行访问
// 2. 接着沿着右上方向返回，递归地访问所有右姊妹
template<typename T, typename Container = BinaryTree<T>>
class PreOrderTraverseLR : public clazy_framework::BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        if (pos != nullptr) {
            while (visit(pos->data()), pos->left() != nullptr) {
                pos = pos->left();
            }
            while (traverse(pos->right(), visit), pos->isLeft()) {
                pos = pos->parent();
            }
        }
    }
};

}
