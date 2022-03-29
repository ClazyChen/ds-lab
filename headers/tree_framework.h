#pragma once

#include "framework.h"
#include "tree_node_framework.h"

namespace clazy_framework {

// 基本的树的类，很多不做实现
template <typename T>
class AbstractTree : public DataStructure<T> {
public:
    // 您需要获取树的规模
    virtual int size() const = 0;

    // 创建节点
    virtual TreeNodePos<T> create() = 0;
    virtual TreeNodePos<T> create(const T& e) = 0;

    // 您需要获取和设置树的根
    virtual TreeNodePos<T> root() const = 0;
    virtual void setRoot(TreeNodePos<T> root) = 0;

    // 一些不需要您自己定义的接口
    virtual bool empty() const override {
        return root() == nullptr;
    }

    virtual void clear() override {
        remove(root());
    }

    // 析构函数，删除整棵树的节点
    virtual ~AbstractTree() { clear(); }

    // 比较接口，使用根节点的值进行比较，这里需要保证两棵树都非空，所以不能使用航天飞机运算符了
    bool operator==(const AbstractTree<T>& other) const { return root() != nullptr && other.root() != nullptr && root()->data() == other.root()->data(); }
    bool operator!=(const AbstractTree<T>& other) const { return !(*this == other); }
    bool operator<(const AbstractTree<T>& other) const { return root() != nullptr && other.root() != nullptr && root()->data() < other.root()->data(); }
    bool operator<=(const AbstractTree<T>& other) const { return !(other < *this); }
    bool operator>(const AbstractTree<T>& other) const { return other < *this; }
    bool operator>=(const AbstractTree<T>& other) const { return !(*this < other); }
};

}