#pragma once

#include "queue.h"
#include "binary_tree.h"
#include "binary_tree_traverse_framework.h"

// 这个文件展示了层次遍历的实现
// 它和先序遍历的SLI形式具有高度一致性

namespace clazy {

template <typename T, typename Container = BinaryTree<T>>
class LevelOrderTraverse : public BinaryTreeTraverse<T, Container> {
protected:
    void traverse(BinaryTreeNodePos<T> pos, const function<void(T&)>& visit) override {
        Queue<BinaryTreeNodePos<T>> Q;
        Q.enqueue(pos);
        while (!Q.empty()) {
            pos = Q.dequeue();
            if (pos != nullptr) {
                visit(pos->data());
                Q.enqueue(pos->left());
                Q.enqueue(pos->right());
            }
        }
    }
};

}
