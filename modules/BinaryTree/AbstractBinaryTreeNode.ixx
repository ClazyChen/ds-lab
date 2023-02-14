module;
#include <memory>

export module BinaryTree.AbstractBinaryTreeNode;

export namespace dslab {

template <typename T>
class AbstractBinaryTreeNode {
public:
    virtual T& data() = 0;
    virtual std::unique_ptr<AbstractBinaryTreeNode>& left() = 0;
    virtual std::unique_ptr<AbstractBinaryTreeNode>& right() = 0;
    virtual AbstractBinaryTreeNode*& parent() = 0;

    virtual bool isRoot() const = 0;
    virtual bool isLeaf() const = 0;
    virtual bool isLeftChild() const = 0;
    virtual bool isRightChild() const = 0;
    virtual AbstractBinaryTreeNode* leftSibling() = 0;
    virtual AbstractBinaryTreeNode* rightSibling() = 0;
};

}