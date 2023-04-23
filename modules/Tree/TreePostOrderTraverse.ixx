module;
#include <functional>

export module Tree.Traverse.TreePostOrderTraverse;

import Tree.TreeNode;
import Tree.Traverse.AbstractTreeTraverse;

export namespace dslab {

template <typename T>
class TreePostOrderTraverse : public AbstractTreeTraverse<T> {
public:
    void operator()(TreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        if (!p) {
            return;
        }
        for (auto& child : p->children()) {
            operator()(child, visit);
        }
        this->call(visit, p);
    }
};

}