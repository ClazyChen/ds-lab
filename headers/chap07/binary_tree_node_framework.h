#pragma once

// 这个文件展示了一个二叉树的节点
// 在这个实现中，我们同时考虑二叉节点和三叉节点（包括亲指针）
// 考虑到现在二叉节点的作用不大（比如，删除节点时会遇到困难）
// 所以在实验中实际上只使用了三叉节点

#include "framework.h"

namespace clazy_framework {

// 二叉树节点的基类
template <typename T>
class AbstractBinaryTreeNode : public Object {
protected:
    T _data;
public:
    using P = AbstractBinaryTreeNode<T>*;

    // 左子和右子的获取和设置接口
    virtual P left() const = 0;
    virtual P right() const = 0;
    virtual void setLeft(P left) = 0;
    virtual void setRight(P right) = 0;

    // 亲节点的获取和设置接口
    virtual P parent() const = 0;
    virtual void setParent(P parent) = 0;

    // 获取数据的接口
    T& data() { return _data; }

    // 判断是左子还是右子
    bool isLeft() const {
        return this->parent() != nullptr && this->parent()->left() == this;
    }
    bool isRight() const {
        return this->parent() != nullptr && this->parent()->right() == this;
    }

    // 判断是否有左姊妹或右姊妹，并获取之
    bool hasLeftSibling() const {
        return this->isRight() && this->parent()->left() != nullptr;
    }
    bool hasRightSibling() const {
        return this->isLeft() && this->parent()->right() != nullptr;
    }
    P leftSibling() const {
        return this->hasLeftSibling() ? this->parent()->left() : nullptr;
    }
    P rightSibling() const {
        return this->hasRightSibling() ? this->parent()->right() : nullptr;
    }

    // 判断是否是根节点或叶节点
    bool isRoot() const {
        return this->parent() == nullptr;
    }
    bool isLeaf() const {
        return this->left() == nullptr && this->right() == nullptr;
    }

    // 获取节点的度数
    int degree() const {
        return (this->left() != nullptr ? 1 : 0) + (this->right() != nullptr ? 1 : 0);
    }

    // 脱离父节点
    void detach() {
        if (this->parent() != nullptr) {
            if (this->isLeft()) {
                this->parent()->setLeft(nullptr);
            } else if (this->isRight()) {
                this->parent()->setRight(nullptr);
            }
            this->setParent(nullptr);
        }
    }

    // 默认实现的构造函数
    AbstractBinaryTreeNode() {}
    AbstractBinaryTreeNode(T data): _data(data) {}

    // 默认实现的比较接口
    bool operator==(const AbstractBinaryTreeNode<T>& other) const { return _data == other._data; }
    auto operator<=>(const AbstractBinaryTreeNode<T>& other) const = default;
};

// 二叉树使用指针表示位置
template <typename T>
using BinaryTreeNodePos = AbstractBinaryTreeNode<T>::P;

// 单向（二叉）节点。子代表示法
template <typename T>
class ForwardBinaryTreeNode : public AbstractBinaryTreeNode<T> {
protected:
    BinaryTreeNodePos<T> _left = nullptr;
    BinaryTreeNodePos<T> _right = nullptr;

    // 隐藏获取和设置亲代的接口
    BinaryTreeNodePos<T> parent() const override { return nullptr; }
    void setParent(BinaryTreeNodePos<T> parent) override {}

public:
    ForwardBinaryTreeNode() {}
    ForwardBinaryTreeNode(T data): AbstractBinaryTreeNode<T>(data) {}

    BinaryTreeNodePos<T> left() const override { return _left; }
    BinaryTreeNodePos<T> right() const override { return _right; }
    void setLeft(BinaryTreeNodePos<T> left) override { _left = left; }
    void setRight(BinaryTreeNodePos<T> right) override { _right = right; }
};

// 双向（三叉）节点。亲代+子代表示法
template <typename T>
class BinaryTreeNode : public ForwardBinaryTreeNode<T> {
protected:
    BinaryTreeNodePos<T> _parent = nullptr;

public:
    BinaryTreeNode() {}
    BinaryTreeNode(T data): ForwardBinaryTreeNode<T>(data) {}

    BinaryTreeNodePos<T> parent() const override { return _parent; }
    void setParent(BinaryTreeNodePos<T> parent) override { _parent = parent; }

    void setLeft(BinaryTreeNodePos<T> left) override { 
        ForwardBinaryTreeNode<T>::setLeft(left);
        if (left != nullptr) {
            left->setParent(this);
        }
    }

    void setRight(BinaryTreeNodePos<T> right) override {
        ForwardBinaryTreeNode<T>::setRight(right);
        if (right != nullptr) {
            right->setParent(this);
        }
    }
};

}