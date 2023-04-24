module;
#include <functional>

export module Tree.Traverse.TreePostOrderTraverse;

import Tree.TreeNode;
import Tree.Traverse.AbstractTreeTraverse;

export namespace dslab {

template <typename T>
class TreePostOrderTraverse : public AbstractTreeTraverse<T> {
public:
    void traverse(TreeNodeConstPos<T> p, std::function<void(TreeNodeConstPos<T>)> visit) override {
        if (!p) {
            return;
        }
        for (auto& child : p->children()) {
            traverse(child, visit);
        }
        visit(p);
    }
};

}