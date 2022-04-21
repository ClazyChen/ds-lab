#pragma once

#include "binary_tree_iterator_framework.h"

// 这个文件给出了中序遍历的迭代器算法

namespace clazy {

using clazy_framework::BinaryTreeNodePos;

template <typename T>
class InOrderIterator : public clazy_framework::BinaryTreeIterateProblem<T> {
public:
    // 中序遍历的直接后继
    // 1. 有右子，右下一步，左下到底
    // 2. 无右子，左上到底，右上一步
    BinaryTreeNodePos<T> succ(BinaryTreeNodePos<T> pos) const override {
        if (pos->right() != nullptr) {
            pos = pos->right();
            while (pos->left() != nullptr) {
                pos = pos->left();
            }
        } else {
            while (pos->isRight()) {
                pos = pos->parent();
            }
            pos = pos->parent();
        }
        return pos;
    }

    // 中序遍历的直接前驱
    // 1. 有左子，左下一步，右下到底
    // 2. 无左子，右上到底，左上一步
    BinaryTreeNodePos<T> pred(BinaryTreeNodePos<T> pos) const override {
        if (pos->left() != nullptr) {
            pos = pos->left();
            while (pos->right() != nullptr) {
                pos = pos->right();
            }
        } else {
            while (pos->isLeft()) {
                pos = pos->parent();
            }
            pos = pos->parent();
        }
        return pos;
    }

    // 中序遍历的第一个位置（左下到底）
    BinaryTreeNodePos<T> first(BinaryTreeNodePos<T> pos) const override {
        if (pos != nullptr) {
            while (pos->left() != nullptr) {
                pos = pos->left();
            }
        }
        return pos;
    }

    // 中序遍历的最后一个位置（右上到底）
    BinaryTreeNodePos<T> last(BinaryTreeNodePos<T> pos) const override {
        if (pos != nullptr) {
            while (pos->right() != nullptr) {
                pos = pos->right();
            }
        }
        return pos;
    }
};

}