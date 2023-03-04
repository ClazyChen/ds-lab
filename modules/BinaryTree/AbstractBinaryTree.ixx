module;
#include <memory>

export module BinaryTree.AbstractBinaryTree;

import BinaryTree.BinaryTreeNode;
import BinaryTree.AbstractBinaryTreeNode;

import Framework.DataStructure;

export namespace dslab {

template <typename T, template <typename> typename Node = BinaryTreeNode>
    requires std::is_base_of_v<AbstractBinaryTreeNode<T>, Node<T>>
class AbstractBinaryTree : public DataStructure<T> {
public:
    virtual Node<T>* root() = 0;
    virtual size_t size() const = 0;
    virtual Node<T>* insertAsLeftChild(Node<T>* node, const T& e) = 0;
    virtual Node<T>* insertAsRightChild(Node<T>* node, const T& e) = 0;
    virtual Node<T>* insertAsLeftChild(Node<T>* node, T&& e) = 0;
    virtual Node<T>* insertAsRightChild(Node<T>* node, T&& e) = 0;
    virtual Node<T>* insertAsRoot(const T& e) = 0;
    virtual Node<T>* insertAsRoot(T&& e) = 0;
    virtual Node<T>* attachAsLeftChild(Node<T>* node, AbstractBinaryTree<T, Node>* tree) = 0;
    virtual Node<T>* attachAsRightChild(Node<T>* node, AbstractBinaryTree<T, Node>* tree) = 0;
    virtual Node<T>* find(const T& e) = 0;
    virtual T remove(Node<T>* node) = 0;
    virtual std::unique_ptr<AbstractBinaryTreeNode<T>> detach(Node<T>* node) = 0;
    virtual void clear() = 0;
};

}
