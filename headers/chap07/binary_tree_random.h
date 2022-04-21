#pragma once

#include "random_framework.h"
#include "binary_tree.h"

// 这个文件讨论如何随机生成一棵二叉树
// 下面使用的算法称为Rémy算法
// Rémy算法考虑的是装饰树：外部节点被标号的增强二叉树
// n阶装饰树的数量为 D[n] = (n+1)! * Catalan(n) = (2n)!/n!
// 因而它的递推关系为 D[n] = (4n-2) * D[n-1]
// 递推方法：
// 1. 初始：只有一个编号为0的节点（外部节点）
// 2. D[n-1] -> D[n]
//    随机选择装饰树上的一个节点v（2n-1种可能）
//    在该节点的位置处，插入一个新节点x
//    令编号为n的外部节点作为节点x的一个子树（2种可能）
//    之前随机选的节点v作为x的另一个子树
// 3. 最后删除所有的外部节点，得到二叉树

namespace clazy {

// 生成一棵随机二叉树
template <typename T, typename Node = clazy_framework::BinaryTreeNode<T>>
requires (is_base_of_v<clazy_framework::AbstractBinaryTreeNode<T>, Node>)
class RandomBinaryTree : public clazy_framework::Algorithm<BinaryTreeNodePos<T>, int> {
public:
    BinaryTreeNodePos<T> apply(int n) override {
        BinaryTreeNodePos<T> root = new Node;         // 虚拟的根节点
        auto A = new BinaryTreeNodePos<T>[2 * n + 1]; // 存储所有的节点
        for (int i = 0; i < 2 * n; i++) {             // 创建所有的节点
            A[i] = new Node;
        }
        root->setLeft(A[0]);                          // 空树的装饰树只有一个节点
        for (int i = 1; i <= n; i++) {
            int r = clazy_framework::Random::nextIntBetween(0, 2*i);
            auto v = A[r / 2];                        // 随机选取一个装饰树中的节点
            auto p = v->parent();                     // 该节点的父节点
            if (v->isLeft()) {                        // 用A[2i-1]替换被选中的节点的位置
                p->setLeft(A[2*i-1]);
            } else {
                p->setRight(A[2*i-1]);
            }
            if (r % 2 == 0) {                         // A[2i-1]下方随机挂载v和A[2i]
                A[2*i-1]->setLeft(v);
                A[2*i-1]->setRight(A[2*i]);
            } else {
                A[2*i-1]->setRight(v);
                A[2*i-1]->setLeft(A[2*i]);
            }
        }
        for (int i = 0; i < n; i++) {                 // 删除所有的外部节点
            if (A[2*i]->isLeft()) {
                A[2*i]->parent()->setLeft(nullptr);
            } else {
                A[2*i]->parent()->setRight(nullptr);
            }
            delete A[2*i];
        }
        root = root->left();                          // 删除虚拟的根节点
        delete root->parent();
        root->setParent(nullptr);
        delete[] A;
        return root;
    }
};

}