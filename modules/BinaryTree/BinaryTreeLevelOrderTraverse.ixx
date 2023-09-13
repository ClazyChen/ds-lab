export module BinaryTree.Traverse.BinaryTreeLevelOrderTraverse;

import BinaryTree.BinaryTreeNode;
import BinaryTree.Traverse.AbstractBinaryTreeTraverse;
import Queue;
import std;

export namespace dslab {

template <typename T>
class BinaryTreeLevelOrderTraverse : public AbstractBinaryTreeTraverse<T> {
public:
    void traverse(BinaryTreeNodeConstPos<T> p, std::function<void(BinaryTreeNodeConstPos<T>)> visit) override {
        Queue<BinaryTreeNodeConstPos<T>> Q { p };
        while (!Q.empty()) {
            if (auto node { Q.dequeue() }; node) {
                visit(node);
                Q.enqueue(node->left());
                Q.enqueue(node->right());
            }
        }
    }
    std::string type_name() const override {
        return "Level Order Traverse";
    }
};

}