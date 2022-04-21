#pragma once

#include "binary_tree_iterator_framework.h"

// 这个文件给出了先序遍历的迭代器算法

namespace clazy {

using clazy_framework::BinaryTreeNodePos;

template <typename T>
class PreOrderIterator : public clazy_framework::BinaryTreeIterateProblem<T> {
public:
    // 先序遍历的直接后继
    // 1. 有左取左，无左取右
    // 2. 左右都无，有右姊妹则取右姊妹，无右姊妹则向上回溯
    //    回溯到根，迭代结束
    BinaryTreeNodePos<T> succ(BinaryTreeNodePos<T> pos) const override {
        if (pos->left() != nullptr) {
            return pos->left();
        }
        if (pos->right() != nullptr) {
            return pos->right();
        }
        while (!pos->isRoot()) {
            if (pos->hasRightSibling()) {
                return pos->rightSibling();
            }
            pos = pos->parent();
        }
        return nullptr;
    }

    // 先序遍历的直接前驱
    // 1. 无左姊妹，则取亲代
    // 2. 有左姊妹，自左姊妹向下到底，优先右下，无右则左
    BinaryTreeNodePos<T> pred(BinaryTreeNodePos<T> pos) const override {
        if (pos->hasLeftSibling()) {
            pos = pos->leftSibling();
            while (!pos->isLeaf()) {
                if (pos->right() != nullptr) {
                    pos = pos->right();
                } else {
                    pos = pos->left();
                }
            }
            return pos;
        } else {
            return pos->parent();
        }
    }

    // 先序遍历的第一个位置
    BinaryTreeNodePos<T> first(BinaryTreeNodePos<T> pos) const override {
        return pos;
    }

    // 先序遍历的最后一个位置
    BinaryTreeNodePos<T> last(BinaryTreeNodePos<T> pos) const override {
        if (pos != nullptr) {
            while (!pos->isLeaf()) {
                if (pos->right() != nullptr) {
                    pos = pos->right();
                } else {
                    pos = pos->left();
                }
            }
        }
        return pos;
    }

};

}