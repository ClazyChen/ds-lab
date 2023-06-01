export module Tree.Traverse.TreeLevelOrderTraverse;

import Tree.TreeNode;
import Tree.Traverse.AbstractTreeTraverse;
import Queue;
import std;

export namespace dslab {

template <typename T>
class TreeLevelOrderTraverse : public AbstractTreeTraverse<T> {
public:
    void traverse(TreeNodeConstPos<T> p, std::function<void(TreeNodeConstPos<T>)> visit) override {
        if (!p) {
            return;
        }
        Queue<TreeNodeConstPos<T>> Q;
        Q.enqueue(p);
        while (!Q.empty()) {
            auto node { Q.dequeue() };
            visit(node);
            for (auto& child : node->children()) {
                Q.enqueue(child);
            }
        }
    }
};

}