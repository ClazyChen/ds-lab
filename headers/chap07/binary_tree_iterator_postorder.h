#pragma once

#include "binary_tree_iterator_framework.h"

// 这个文件给出了后序遍历的迭代器算法
// 从迭代器的角度更加能够显而易见地看出先序、逆后序、后序之间的对称性

namespace clazy {

using clazy_framework::BinaryTreeNodePos;

template <typename T>
class PostOrderIterator : public clazy_framework::BinaryTreeIterateProblem<T> {
public:
    // 后序遍历的直接后继
    // 1. 无右姊妹，则取亲代
    // 2. 有右姊妹，自右姊妹向下到底，优先左下，无左则右
    BinaryTreeNodePos<T> succ(BinaryTreeNodePos<T> pos) const override {
        if (pos->hasRightSibling()) {
            pos = pos->rightSibling();
            while (!pos->isLeaf()) {
                if (pos->left() != nullptr) {
                    pos = pos->left();
                } else {
                    pos = pos->right();
                }
            }
            return pos;
        } else {
            return pos->parent();
        }
    }

    // 后序遍历的直接前驱
    // 1. 有右取右，无右取左
    // 2. 左右都无，有左姊妹则取左姊妹，无左姊妹则向上回溯
    //    回溯到根，迭代结束
    BinaryTreeNodePos<T> pred(BinaryTreeNodePos<T> pos) const override {
        if (pos->right() != nullptr) {
            return pos->right();
        }
        if (pos->left() != nullptr) {
            return pos->left();
        }
        while (!pos->isRoot()) {
            if (pos->hasLeftSibling()) {
                return pos->leftSibling();
            }
            pos = pos->parent();
        }
        return nullptr;
    }

    // 后序遍历的第一个位置
    BinaryTreeNodePos<T> first(BinaryTreeNodePos<T> pos) const override {
        if (pos != nullptr) {
            while (!pos->isLeaf()) {
                if (pos->left() != nullptr) {
                    pos = pos->left();
                } else {
                    pos = pos->right();
                }
            }
        }
        return pos;
    }

    // 后序遍历的最后一个位置
    BinaryTreeNodePos<T> last(BinaryTreeNodePos<T> pos) const override {
        return pos;
    }
};

}