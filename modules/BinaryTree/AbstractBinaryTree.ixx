module;
#include <iostream>

export module BinaryTree.AbstractBinaryTree;

import BinaryTree.BinaryTreeNode;
import Framework.DataStructure;

export namespace dslab {

template <typename T>
class AbstractBinaryTree : public DataStructure<T> {
public:
    virtual BinaryTreeNodePos<T> root() = 0;
    virtual BinaryTreeNodeConstPos<T> root() const = 0;
    virtual size_t size() const = 0;
    virtual BinaryTreeNodePos<T> insertAsLeftChild(BinaryTreeNodePos<T> p, const T& e) = 0;
    virtual BinaryTreeNodePos<T> insertAsRightChild(BinaryTreeNodePos<T> p, const T& e) = 0;
    virtual BinaryTreeNodePos<T> insertAsLeftChild(BinaryTreeNodePos<T> p, T&& e) = 0;
    virtual BinaryTreeNodePos<T> insertAsRightChild(BinaryTreeNodePos<T> p, T&& e) = 0;
    virtual BinaryTreeNodePos<T> insertAsRoot(const T& e) = 0;
    virtual BinaryTreeNodePos<T> insertAsRoot(T&& e) = 0;
    virtual BinaryTreeNodePos<T> attachAsLeftChild(BinaryTreeNodePos<T> p, BinaryTreeNodeInst<T>&& st) = 0;
    virtual BinaryTreeNodePos<T> attachAsRightChild(BinaryTreeNodePos<T> p, BinaryTreeNodeInst<T>&& st) = 0;
    virtual BinaryTreeNodePos<T> find(const T& e) const = 0;
    virtual T remove(BinaryTreeNodePos<T> p) = 0;
    virtual BinaryTreeNodeInst<T> detach(BinaryTreeNodePos<T> p) = 0;
    virtual void clear() = 0;
};

}
