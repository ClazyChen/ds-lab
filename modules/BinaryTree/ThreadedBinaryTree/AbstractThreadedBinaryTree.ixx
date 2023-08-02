export module ThreadedBinaryTree.AbstractThreadedBinaryTree;

import ThreadedBinaryTree.ThreadedBinaryTreeNode;
import BinaryTree.AbstractBinaryTree;

export namespace dslab {

template <typename T>
using AbstractThreadedBinaryTree = AbstractTemplateBinaryTree<T, ThreadedBinaryTreeNode>;

}